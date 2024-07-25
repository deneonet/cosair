#include "crpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"

namespace Cosair {

	ShaderRef Shader::Create(const std::string& filepath) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		}

		CR_CORE_DERROR("API is not supported by the specified platform");
		return nullptr;
	}

	ShaderRef Shader::Create(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(vertexFilepath, fragmentFilepath);
		}

		CR_CORE_DERROR("API is not supported by the specified platform");
		return nullptr;
	}

}