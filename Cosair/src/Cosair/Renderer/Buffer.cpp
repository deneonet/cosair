#include "crpch.h"

#include "buffer.h"

#include "platforms/opengl/opengl_buffer.h"
#include "renderer_api.h"

namespace cosair {

VertexBufferRef VertexBuffer::Create(const void* data, size_t size) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLVertexBuffer>(data, size);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

VertexBufferRef VertexBuffer::Create(size_t size) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLVertexBuffer>(size);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

ShdSBufferRef ShdSBuffer::Create(const void* data, size_t size) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLShdSBuffer>(data, size);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

ShdSBufferRef ShdSBuffer::Create(size_t size) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLShdSBuffer>(size);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

IndexBufferRef IndexBuffer::Create(const void* data, size_t count) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLIndexBuffer>(data, count);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

}  // namespace cosair