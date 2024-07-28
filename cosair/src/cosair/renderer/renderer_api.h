#pragma once

#include <glm/glm.hpp>

#include "vertex_array.h"

namespace cosair {

class RendererApi {
 public:
  enum class Api { kNone = 0, kOpenGL = 1 };

 public:
  virtual void Init() = 0;
  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;

  virtual void Clear() = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;

  virtual void DrawIndexed(size_t indicies_count) = 0;
  virtual void DrawIndexed(const VertexArrayRef& vertex_array) = 0;

  inline static Api GetApi() { return api_; }
  inline static void SetApi(Api api) { api_ = api; }

 private:
  static Api api_;
};

}  // namespace cosair