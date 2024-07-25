#include "crpch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "RendererAPI.h"

namespace Cosair {

	VertexArrayRef VertexArray::Create() {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_ASSERT(false, "No renderer API specified: RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		CR_CORE_ASSERT(false, "API is not supported by the specified platform")
			return nullptr;
	}

}