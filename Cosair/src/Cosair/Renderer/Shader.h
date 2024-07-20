#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Cosair {

	enum class ShaderType : uint8_t {
		Float = 0,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
		Int,
		IVec2,
		IVec3,
		IVec4,
		Bool
	};

	static uint32_t GetShaderTypeSize(ShaderType type) {
		switch (type) {
		case ShaderType::Float:   return 4;
		case ShaderType::Vec2:    return 4 * 2;
		case ShaderType::Vec3:    return 4 * 3;
		case ShaderType::Vec4:    return 4 * 4;
		case ShaderType::Mat3:    return 4 * 3 * 3;
		case ShaderType::Mat4:    return 4 * 4 * 4;
		case ShaderType::Int:     return 4;
		case ShaderType::IVec2:   return 4 * 2;
		case ShaderType::IVec3:   return 4 * 3;
		case ShaderType::IVec4:   return 4 * 4;
		case ShaderType::Bool:    return 1;
		}
		return 0;
	}

	class Shader {
	public:
		virtual ~Shader() {};

		inline virtual void Bind() const = 0;
		inline virtual void Unbind() const = 0;

		inline virtual uint32_t GetId() const = 0;

		// TODO: Rename to not sound opengl specific, like: SetMat4
		virtual void UploadUniformMat4(const char* name, const glm::mat4& matrix) = 0;
		virtual void UploadUniformMat3(const char* name, const glm::mat3& matrix) = 0;

		virtual void UploadUniformVec4(const char* name, const glm::vec4& value) = 0;
		virtual void UploadUniformVec3(const char* name, const glm::vec3& value) = 0;
		virtual void UploadUniformVec2(const char* name, const glm::vec2& value) = 0;
		virtual void UploadUniformFloat(const char* name, float value) = 0;

		virtual void UploadUniformInt4(const char* name, const glm::ivec4& value) = 0;
		virtual void UploadUniformInt3(const char* name, const glm::ivec3& value) = 0;
		virtual void UploadUniformInt2(const char* name, const glm::ivec2& value) = 0;
		virtual void UploadUniformInt(const char* name, int value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	};

	using ShaderRef = Ref<Shader>;
}