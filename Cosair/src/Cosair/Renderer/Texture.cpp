#include "crpch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "RendererAPI.h"

namespace Cosair {

	Texture2dRef Texture2D::Create(const std::string& path) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		CR_CORE_ASSERT(false, "API is not supported by the specified platform")
			return nullptr;
	}

}