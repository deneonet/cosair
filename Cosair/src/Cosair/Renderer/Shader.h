#pragma once

#include <glm/glm.hpp>

namespace Cosair {

	enum class ShaderType : uint8_t {
		Float = 0,
		Float2,
		Float3,
		Float4,
		Matrix3,
		Matrix4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t GetShaderTypeSize(ShaderType type) {
		switch (type) {
		case ShaderType::Int:
		case ShaderType::Float:   return 4;
		case ShaderType::Bool:    return 1;
		case ShaderType::Int2:
		case ShaderType::Float2:    return 4 * 2;
		case ShaderType::Int3:
		case ShaderType::Float3:    return 4 * 3;
		case ShaderType::Int4:
		case ShaderType::Float4:    return 4 * 4;
		case ShaderType::Matrix3:    return 4 * 3 * 3;
		case ShaderType::Matrix4:    return 4 * 4 * 4;
		}

		return 0;
	}

	class Shader {
	public:
		virtual ~Shader() { };

		inline virtual void Bind() const = 0;
		inline virtual void Unbind() const = 0;

		inline virtual uint32_t GetProgramId() const = 0;

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

		virtual void SetIntArray(const char* name, int* values, uint32_t count) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	};

	using ShaderRef = Ref<Shader>;
}