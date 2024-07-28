#include "crpch.h"

#include "shader.h"

#include "platforms/opengl/opengl_shader.h"
#include "renderer_api.h"

namespace cosair {

ShaderRef Shader::Create(const std::string& filepath) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLShader>(filepath);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

ShaderRef Shader::Create(const std::string& vertex_filepath,
                         const std::string& fragment_filepath) {
  switch (RendererApi::GetApi()) {
    case RendererApi::Api::kNone:
      CR_CORE_CRITICAL("RendererApi::kNone is not supported");
      return nullptr;
    case RendererApi::Api::kOpenGL:
      return std::make_shared<OpenGLShader>(vertex_filepath, fragment_filepath);
  }

  CR_CORE_CRITICAL("Unknown RendererApi");
  return nullptr;
}

}  // namespace cosair