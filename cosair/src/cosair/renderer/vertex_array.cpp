#include "crpch.h"

#include "vertex_array.h"

#include "platforms/opengl/opengl_vertex_array.h"
#include "renderer_api.h"

namespace cosair {

VertexArrayRef VertexArray::Create() {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLVertexArray>();
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

}  // namespace cosair