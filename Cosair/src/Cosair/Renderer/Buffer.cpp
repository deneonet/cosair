#include "crpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "RendererAPI.h"

namespace Cosair {

	VertexBufferRef VertexBuffer::Create(const void* data, uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(data, size);
		}
		CR_CORE_DERROR("Unknown RendererAPI");
		return nullptr;
	}

	VertexBufferRef VertexBuffer::Create(uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size);
		}
		CR_CORE_DERROR("Unknown RendererAPI");
		return nullptr;
	}

	ShaderStorageBufferRef ShaderStorageBuffer::Create(const void* data, uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShaderStorageBuffer>(data, size);
		}
		CR_CORE_DERROR("Unknown RendererAPI");
		return nullptr;
	}

	ShaderStorageBufferRef ShaderStorageBuffer::Create(uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShaderStorageBuffer>(size);
		}
		CR_CORE_DERROR("Unknown RendererAPI");
		return nullptr;
	}

	IndexBufferRef IndexBuffer::Create(const uint32_t* data, uint32_t count) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			CR_CORE_DERROR("RendererAPI::None is currently not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(data, count);
		}
		CR_CORE_DERROR("Unknown RendererAPI");
		return nullptr;
	}

}