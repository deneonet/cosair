#pragma once

#include "material.h"
#include "renderer_2d.h"
#include "renderer_api.h"

namespace cosair {

class RenderOps {
 public:
  inline static void Init() {
    renderer_api_->Init();

    Material::Init();
    Renderer2D::Init();
  }

  inline static void Shutdown() { Renderer2D::Shutdown(); }

  inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
    renderer_api_->SetViewport(x, y, width, height);
  }

  inline static void SetClearColor(const glm::vec4& color) {
    renderer_api_->SetClearColor(color);
  }

  inline static void Clear() { renderer_api_->Clear(); }

  // Draws the count of indicies provided
  inline static void DrawIndexed(size_t indicies_count) {
    renderer_api_->DrawIndexed(indicies_count);
  }

  // Draws the entire count of indicies inside the vertex array
  inline static void DrawIndexed(const VertexArrayRef& vertex_array) {
    renderer_api_->DrawIndexed(vertex_array);
  }

 private:
  static Scope<RendererApi> renderer_api_;
};

}  // namespace cosair