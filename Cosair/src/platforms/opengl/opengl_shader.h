#pragma once

#include <glad/glad.h>

#include "cosair/renderer/shader.h"

namespace cosair {

static GLenum GetOpenGLBaseType(ShaderType type) {
  switch (type) {
    case ShaderType::kBool:
      return GL_BOOL;
    case ShaderType::kInt:
    case ShaderType::kInt2:
    case ShaderType::kInt3:
    case ShaderType::kInt4:
      return GL_INT;
    case ShaderType::kFloat:
    case ShaderType::kFloat2:
    case ShaderType::kFloat3:
    case ShaderType::kFloat4:
    case ShaderType::kMatrix3:
    case ShaderType::kMatrix4:
      return GL_FLOAT;
  }
  return 0;
}

class OpenGLShader : public Shader {
 public:
  OpenGLShader(const std::string& filepath);
  // Both shader files/sources may not contain any '#type' annotation, just raw
  // shader source
  OpenGLShader(const std::string& vertex_filepath,
               const std::string& fragment_filepath);

  virtual ~OpenGLShader() override;

  virtual void Bind() override;
  virtual void Unbind() override;

  inline virtual uint32_t GetId() const override { return opengl_id_; }

  virtual void SetMatrix4(const char* name, const glm::mat4& matrix) override;
  virtual void SetMatrix3(const char* name, const glm::mat3& matrix) override;

  virtual void SetFloat4(const char* name, const glm::vec4& value) override;
  virtual void SetFloat3(const char* name, const glm::vec3& value) override;
  virtual void SetFloat2(const char* name, const glm::vec2& value) override;
  virtual void SetFloat(const char* name, float value) override;

  virtual void SetInt4(const char* name, const glm::ivec4& value) override;
  virtual void SetInt3(const char* name, const glm::ivec3& value) override;
  virtual void SetInt2(const char* name, const glm::ivec2& value) override;
  virtual void SetInt(const char* name, int value) override;

  virtual void SetIntArray(const char* name, int* values,
                           size_t count) override;

 private:
  struct ShaderInfo {
    std::string filepath;
    std::string source;
  };

 private:
  // The fallback fragment shader shows a purple color, indicating that
  // fragment shader compilation failed
  GLuint CreateFallbackShader();

  // TODO: Put into cross-platform file utility class
  std::string ReadFile(const std::string& filepath);
  void Compile(const std::unordered_map<GLenum, ShaderInfo>& shader_sources);
  std::unordered_map<GLenum, ShaderInfo> PreProcess(
      const std::string& src, const std::string& filepath);

 private:
  uint32_t opengl_id_;
};

}  // namespace cosair