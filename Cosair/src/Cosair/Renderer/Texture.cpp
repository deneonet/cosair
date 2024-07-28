#include "crpch.h"

#include "texture.h"

#include "platforms/opengl/opengl_texture.h"
#include "renderer_api.h"

namespace cosair {

Texture2dRef Texture2D::Create(const std::string& path) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

}  // namespace cosair