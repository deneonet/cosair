#include "crpch.h"

#include "opengl_shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace cosair {

static GLenum GetOpenGLShaderType(const std::string& type) {
  if (type == "vertex") return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
  CR_CORE_CRITICAL("Invalid type '{0}' to get as OpenGL shader type", type);
  return 0;
}

static std::string GetOpenGLShaderName(GLenum type) {
  if (type == GL_VERTEX_SHADER) return "Vertex";
  if (type == GL_FRAGMENT_SHADER) return "Fragment";
  CR_CORE_CRITICAL("Invalid type '{0}' to get as name", type);
  return "";
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
  CR_PROFILE_FUNCTION();
  const std::string& source = ReadFile(filepath);
  Compile(PreProcess(source, filepath));
}

OpenGLShader::OpenGLShader(const std::string& vertex_filepath,
                           const std::string& fragment_filepath) {
  CR_PROFILE_FUNCTION();
  const std::string& vertex_source = ReadFile(vertex_filepath);
  const std::string& fragment_source = ReadFile(fragment_filepath);

  std::unordered_map<GLenum, ShaderInfo> shader_sources;
  shader_sources[GL_VERTEX_SHADER] = ShaderInfo{
      vertex_filepath,
      vertex_source,
  };
  shader_sources[GL_FRAGMENT_SHADER] = ShaderInfo{
      fragment_filepath,
      fragment_source,
  };

  Compile(shader_sources);
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, ShaderInfo>& shader_sources) {
  CR_PROFILE_FUNCTION();
  CR_CORE_ASSERT(
      shader_sources.size() <= 4,
      "Exceeding max amount (4) of shaders for each file containing shaders");

  GLuint program = glCreateProgram();
  GLuint shader_ids[4];

  int size = 0;
  for (const std::pair<GLenum, ShaderInfo>& shader_source : shader_sources) {
    GLenum type = shader_source.first;
    const std::string& source = shader_source.second.source;
    const std::string& filepath = shader_source.second.filepath;

    GLuint shader = glCreateShader(type);
    const GLchar* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, 0);
    glCompileShader(shader);

    GLint compiled_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled_status);
    if (compiled_status == GL_FALSE) {
      GLint max_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

      std::vector<GLchar> info_log(max_length);
      glGetShaderInfoLog(shader, max_length, nullptr, &info_log[0]);

      glDeleteShader(shader);

      if (type == GL_VERTEX_SHADER) {
        CR_CORE_TRACE("{0}", info_log.data());
        CR_CORE_CRITICAL("Vertex shader compilation failure in '{0}'",
                         filepath);
        break;
      }

      CR_CORE_TRACE("{0}", info_log.data());
      CR_CORE_ERROR("Fragment shader compilation failure in '{0}'", filepath);
      CR_CORE_ERROR("Using fallback fragment shader\n");
      shader = CreateFallbackShader();
    }

    glAttachShader(program, shader);
    shader_ids[size] = shader;
    size++;
  }

  opengl_id_ = program;
  glLinkProgram(opengl_id_);

  GLint linking_status = 0;
  glGetProgramiv(opengl_id_, GL_LINK_STATUS, &linking_status);
  if (linking_status == GL_FALSE) {
    GLint max_length = 0;
    glGetProgramiv(opengl_id_, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<GLchar> info_log(max_length);
    glGetProgramInfoLog(opengl_id_, max_length, &max_length, &info_log[0]);

    glDeleteProgram(opengl_id_);
    for (int i = 0; i < size; i++) {
      glDeleteShader(shader_ids[i]);
    }

    CR_CORE_TRACE("{0}", info_log.data());
    CR_CORE_CRITICAL("Program linking failure.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    uint32_t id = shader_ids[i];
    glDetachShader(opengl_id_, id);
    glDeleteShader(id);
  }
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(opengl_id_); }

void OpenGLShader::Bind() { glUseProgram(opengl_id_); }
void OpenGLShader::Unbind() { glUseProgram(0); }

GLuint OpenGLShader::CreateFallbackShader() {
  CR_PROFILE_FUNCTION();

  static const GLchar* source = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main() {
				color = vec4(1, 0, 1, 1);
			}
		)";

  GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader, 1, &source, 0);
  glCompileShader(shader);

  GLint compiled_status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled_status);
  if (compiled_status == GL_FALSE) {
    GLint max_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<GLchar> info_log(max_length);
    glGetShaderInfoLog(shader, max_length, nullptr, &info_log[0]);

    glDeleteShader(shader);
    CR_CORE_TRACE("{0}", info_log.data());
    CR_CORE_CRITICAL(
        "Fallback Fragment shader compilation failure. >>> Should be "
        "reported...");
  }

  return shader;
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
  CR_PROFILE_FUNCTION();

  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
      in.close();
      return result;
    }

    CR_CORE_CRITICAL("Could not read from file '{0}'", filepath);
    return "";
  }

  CR_CORE_CRITICAL("Could not open file '{0}'", filepath);
  return "";
}

std::unordered_map<GLenum, OpenGLShader::ShaderInfo> OpenGLShader::PreProcess(
    const std::string& source, const std::string& filepath) {
  CR_PROFILE_FUNCTION();
  std::unordered_map<GLenum, ShaderInfo> shader_sources;

  const char* type_token = "#type";
  size_t type_token_length = strlen(type_token);
  size_t pos = source.find(type_token, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);
    if (eol == std::string::npos) {
      CR_CORE_ERROR("Syntax Error in '{0}'\n", filepath);
      return {};
    }

    size_t begin = pos + type_token_length + 1;
    std::string type = source.substr(begin, eol - begin);

    if (type != "vertex" && type != "fragment" && type != "pixel") {
      CR_CORE_ERROR(
          "Invalid shader type specification (allowed: 'vertex', 'fragment', "
          "'pixel') in '{0}'\n",
          filepath);
      return {};
    }

    size_t next_line_pos = source.find_first_not_of("\r\n", eol);
    if (next_line_pos == std::string::npos) {
      CR_CORE_ERROR("Syntax Error in '{0}'\n", filepath);
      return {};
    }

    pos = source.find(type_token, next_line_pos);
    shader_sources[GetOpenGLShaderType(type)] = ShaderInfo{
        filepath, (pos == std::string::npos)
                      ? source.substr(next_line_pos)
                      : source.substr(next_line_pos, pos - next_line_pos)};
  }
  return shader_sources;
}

void OpenGLShader::SetMatrix4(const char* name, const glm::mat4& matrix) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::SetMatrix3(const char* name, const glm::mat3& matrix) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::SetFloat4(const char* name, const glm::vec4& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetFloat3(const char* name, const glm::vec3& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::SetFloat2(const char* name, const glm::vec2& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform2f(location, value.x, value.y);
}

void OpenGLShader::SetFloat(const char* name, float value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform1f(location, value);
}

void OpenGLShader::SetInt4(const char* name, const glm::ivec4& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform4i(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetInt3(const char* name, const glm::ivec3& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform3i(location, value.x, value.y, value.z);
}

void OpenGLShader::SetInt2(const char* name, const glm::ivec2& value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform2i(location, value.x, value.y);
}

void OpenGLShader::SetInt(const char* name, int value) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform1i(location, value);
}

void OpenGLShader::SetIntArray(const char* name, int* values, size_t count) {
  CR_PROFILE_FUNCTION();
  uint32_t location = glGetUniformLocation(opengl_id_, name);
  glUniform1iv(location, (GLsizei)count, values);
}

}  // namespace cosair