#include "crpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cosair {

	static GLenum GetOpenGLShaderType(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		CR_CORE_ASSERT(false, "Invalid type '{0}' to get as opengl shader type", type);
		return 0;
	}

	static std::string GetOpenGLShaderName(GLenum type) {
		if (type == GL_VERTEX_SHADER) return "Vertex";
		if (type == GL_FRAGMENT_SHADER) return "Fragment";
		CR_CORE_ASSERT(false, "Invalid type '{0}' to get as name", type);
		return "";
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) : m_ProgramId(0) {
		CR_PROFILE_FUNCTION();
		const std::string& source = ReadFile(filepath);
		Compile(PreProcess(source, filepath));
	}

	// Both shader files/sources may not contain any '#type' annotation, just raw shader source
	OpenGLShader::OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath) : m_ProgramId(0) {
		CR_PROFILE_FUNCTION();
		const std::string& vertexSource = ReadFile(vertexFilepath);
		const std::string& fragmentSource = ReadFile(fragmentFilepath);

		std::unordered_map<GLenum, ShaderInfo> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = ShaderInfo {
			vertexFilepath,
			vertexSource,
		};
		shaderSources[GL_FRAGMENT_SHADER] = ShaderInfo {
			fragmentFilepath,
			fragmentSource,
		};

		Compile(shaderSources);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, ShaderInfo>& shaderSources) {
		CR_PROFILE_FUNCTION();
		CR_CORE_ASSERT(shaderSources.size() <= 4, "Exceeding max amount (4) of shaders for each file containing shaders");

		GLuint program = glCreateProgram();
		GLuint shadersIDs[4];

		int size = 0;
		for (auto& shaderSrc : shaderSources) {
			GLenum type = shaderSrc.first;
			const std::string& source = shaderSrc.second.source;
			const std::string& filepath = shaderSrc.second.filepath;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint compiledStatus = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
			if (compiledStatus == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::string log;
				log.resize(maxLength);
				glGetShaderInfoLog(shader, maxLength, nullptr, &log[0]);

				glDeleteShader(shader);

				if (type == GL_VERTEX_SHADER) {
					CR_CORE_TRACE("{0}", log);
					CR_CORE_DERROR("Vertex shader compilation failure in '{0}'", filepath);
					break;
				}

				CR_CORE_TRACE("{0}", log);
				CR_CORE_ERROR("Fragment shader compilation failure in '{0}'", filepath);
				CR_CORE_ERROR("Using fallback fragment shader\n");
				shader = CreateFallbackShader();
			}

			glAttachShader(program, shader);
			shadersIDs[size] = shader;
			size++;
		}

		m_ProgramId = program;
		glLinkProgram(m_ProgramId);

		GLint linkingStatus = 0;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &linkingStatus);
		if (linkingStatus == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ProgramId);
			for (int i = 0; i < size; i++) {
				glDeleteShader(shadersIDs[i]);
			}

			CR_CORE_TRACE("{0}", infoLog.data());
			CR_CORE_DERROR("Program linking failure.\n");
			return;
		}

		for (int i = 0; i < size; i++) {
			uint32_t id = shadersIDs[i];
			glDetachShader(m_ProgramId, id);
			glDeleteShader(id);
		}
	}

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

		GLint compiledStatus = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);
		if (compiledStatus == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::string log;
			log.resize(maxLength);
			glGetShaderInfoLog(shader, maxLength, nullptr, &log[0]);

			glDeleteShader(shader);
			CR_CORE_TRACE("{0}", log);
			CR_CORE_DERROR("Fallback Fragment shader compilation failure. >>> Should be reported...");
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

			CR_CORE_ASSERT("Could not read from file '{0}'", filepath);
			return "";
		}

		CR_CORE_ASSERT(false, "Could not open file '{0}'", filepath);
		return "";
	}

	std::unordered_map<GLenum, OpenGLShader::ShaderInfo> OpenGLShader::PreProcess(const std::string& source, const std::string& filepath) {
		CR_PROFILE_FUNCTION();
		std::unordered_map<GLenum, ShaderInfo> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == std::string::npos) {
				CR_CORE_ERROR("Syntax Error in '{0}'\n", filepath);
				return {};
			}

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			if (type != "vertex" && type != "fragment" && type != "pixel") {
				CR_CORE_ERROR("Invalid shader type specification (allowed: 'vertex', 'fragment', 'pixel') in '{0}'\n", filepath);
				return {};
			}

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			if (nextLinePos == std::string::npos) {
				CR_CORE_ERROR("Syntax Error in '{0}'\n", filepath);
				return {};
			}

			pos = source.find(typeToken, nextLinePos);
			shaderSources[GetOpenGLShaderType(type)] = ShaderInfo {
				filepath,
				(pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos)
			};
		}
		return shaderSources;
	}

	void OpenGLShader::SetMatrix4(const char* name, const glm::mat4& matrix) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMatrix3(const char* name, const glm::mat3& matrix) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetFloat4(const char* name, const glm::vec4& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetFloat3(const char* name, const glm::vec3& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat2(const char* name, const glm::vec2& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat(const char* name, float value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform1f(location, value);
	}

	void OpenGLShader::SetInt4(const char* name, const glm::ivec4& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetInt3(const char* name, const glm::ivec3& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform3i(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetInt2(const char* name, const glm::ivec2& value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform2i(location, value.x, value.y);
	}

	void OpenGLShader::SetInt(const char* name, int value) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const char* name, int* values, uint32_t count) {
		CR_PROFILE_FUNCTION();
		uint32_t location = glGetUniformLocation(m_ProgramId, name);
		glUniform1iv(location, count, values);
	}

}