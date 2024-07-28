#pragma once

#include <glm/glm.hpp>

namespace cosair {

enum ShaderType : uint8_t {
  kFloat = 0,
  kFloat2,
  kFloat3,
  kFloat4,
  kMatrix3,
  kMatrix4,
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kBool
};

static uint32_t GetShaderTypeSize(ShaderType type) {
  switch (type) {
    case ShaderType::kInt:
    case ShaderType::kFloat:
      return 4;
    case ShaderType::kBool:
      return 1;
    case ShaderType::kInt2:
    case ShaderType::kFloat2:
      return 4 * 2;
    case ShaderType::kInt3:
    case ShaderType::kFloat3:
      return 4 * 3;
    case ShaderType::kInt4:
    case ShaderType::kFloat4:
      return 4 * 4;
    case ShaderType::kMatrix3:
      return 4 * 3 * 3;
    case ShaderType::kMatrix4:
      return 4 * 4 * 4;
  }

  return 0;
}

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  inline virtual uint32_t GetId() const = 0;

  virtual void SetMatrix4(const char* name, const glm::mat4& matrix) = 0;
  virtual void SetMatrix3(const char* name, const glm::mat3& matrix) = 0;

  virtual void SetFloat4(const char* name, const glm::vec4& value) = 0;
  virtual void SetFloat3(const char* name, const glm::vec3& value) = 0;
  virtual void SetFloat2(const char* name, const glm::vec2& value) = 0;
  virtual void SetFloat(const char* name, float value) = 0;

  virtual void SetInt4(const char* name, const glm::ivec4& value) = 0;
  virtual void SetInt3(const char* name, const glm::ivec3& value) = 0;
  virtual void SetInt2(const char* name, const glm::ivec2& value) = 0;
  virtual void SetInt(const char* name, int value) = 0;

  virtual void SetIntArray(const char* name, int* values, size_t count) = 0;

  static Ref<Shader> Create(const std::string& filepath);
  static Ref<Shader> Create(const std::string& vertex_filepath,
                            const std::string& fragment_filepath);
};

using ShaderRef = Ref<Shader>;
}  // namespace cosair