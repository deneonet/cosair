#include "crpch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Cosair {

	ShaderRef Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		}

		CR_CORE_ASSERT(false, "API is not supported by the specified platform")
			return nullptr;
	}

	ShaderRef Shader::Create(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(vertexFilepath, fragmentFilepath);
		}

		CR_CORE_ASSERT(false, "API is not supported by the specified platform")
		return nullptr;
	}

}