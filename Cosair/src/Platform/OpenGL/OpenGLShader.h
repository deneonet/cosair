#pragma once

#include "Cosair/Renderer/Shader.h"

#include <glad/glad.h>

namespace Cosair {

	static GLenum GetOpenGLBaseType(ShaderType type) {
		switch (type) {
		case ShaderType::Bool:      return GL_BOOL;
		case ShaderType::Int:
		case ShaderType::Int2:
		case ShaderType::Int3:
		case ShaderType::Int4:      return GL_INT;
		case ShaderType::Float:
		case ShaderType::Float2:
		case ShaderType::Float3:
		case ShaderType::Float4:
		case ShaderType::Matrix3:
		case ShaderType::Matrix4:      return GL_FLOAT;
		}
		return 0;
	}

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);

		inline virtual ~OpenGLShader() override { glDeleteProgram(m_ProgramId); }

		inline virtual void Bind() const override { glUseProgram(m_ProgramId); }
		inline virtual void Unbind() const override { glUseProgram(0); }

		inline virtual uint32_t GetProgramId() const override { return m_ProgramId; }

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

		virtual void SetIntArray(const char* name, int* values, uint32_t count) override;
	private:
		struct ShaderInfo {
			std::string filepath;
			std::string source;
		};
	private:
		// The fallback fragment shader just shows a purple color, indicating that shader compilation failed
		GLuint CreateFallbackShader();

		// TODO: Put into cross-platform file utility class
		std::string ReadFile(const std::string& filepath);
		void Compile(const std::unordered_map<GLenum, ShaderInfo>& shaderSources);
		std::unordered_map<GLenum, ShaderInfo> PreProcess(const std::string& src, const std::string& filepath);
	private:
		uint32_t m_ProgramId;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};

}