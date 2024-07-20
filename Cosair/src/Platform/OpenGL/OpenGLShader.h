#pragma once

#include "glad/glad.h"
#include "Cosair/Renderer/Shader.h"

namespace Cosair {

	static GLenum GetOpenGLBaseType(ShaderType type) {
		switch (type) {
		case ShaderType::Float:     return GL_FLOAT;
		case ShaderType::Vec2:    return GL_FLOAT;
		case ShaderType::Vec3:    return GL_FLOAT;
		case ShaderType::Vec4:    return GL_FLOAT;
		case ShaderType::Mat3:      return GL_FLOAT;
		case ShaderType::Mat4:      return GL_FLOAT;
		case ShaderType::Int:       return GL_INT;
		case ShaderType::IVec2:      return GL_INT;
		case ShaderType::IVec3:      return GL_INT;
		case ShaderType::IVec4:      return GL_INT;
		case ShaderType::Bool:      return GL_BOOL;
		}
		return 0;
	}

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);

		inline virtual ~OpenGLShader() override { glDeleteProgram(m_ProgramID); }

		inline virtual void Bind() const override { glUseProgram(m_ProgramID); }
		inline virtual void Unbind() const override { glUseProgram(0); }

		inline virtual uint32_t GetId() const override { return m_ProgramID; }

		virtual void UploadUniformMat4(const char* name, const glm::mat4& matrix) override;
		virtual void UploadUniformMat3(const char* name, const glm::mat3& matrix) override;

		virtual void UploadUniformVec4(const char* name, const glm::vec4& value) override;
		virtual void UploadUniformVec3(const char* name, const glm::vec3& value) override;
		virtual void UploadUniformVec2(const char* name, const glm::vec2& value) override;
		virtual void UploadUniformFloat(const char* name, float value) override;

		virtual void UploadUniformInt4(const char* name, const glm::ivec4& value) override;
		virtual void UploadUniformInt3(const char* name, const glm::ivec3& value) override;
		virtual void UploadUniformInt2(const char* name, const glm::ivec2& value) override;
		virtual void UploadUniformInt(const char* name, int value) override;
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
		uint32_t m_ProgramID;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};

}