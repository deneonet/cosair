#pragma once

#include <glad/glad.h>

#include "cosair/renderer/renderer_api.h"

namespace cosair {

class OpenGLRendererAPI : public RendererApi {
  virtual void Init() override;

  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) override;

  virtual void Clear() override;
  virtual void SetClearColor(const glm::vec4& color) override;

  virtual void DrawIndexed(size_t indicies_count) override;
  virtual void DrawIndexed(const VertexArrayRef& vertex_array) override;
};

}  // namespace cosair