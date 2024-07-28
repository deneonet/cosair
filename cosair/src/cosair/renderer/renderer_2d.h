#pragma once

#include "cosair/scene/components/camera.h"
#include "cosair/scene/components/transform.h"
#include "material.h"

namespace cosair {

struct RendererProps2D {
  uint32_t max_quads = 10000;
#ifdef CR_EXT_BINDLESS_TEXS
  uint32_t max_bindless_textures = 500000;
#endif
};

class Renderer2D {
 public:
  static void Init(RendererProps2D props = RendererProps2D());
  static void Shutdown();

  static void BeginScene(const glm::mat4& view_proj_matrix);
  static void EndScene();

  static void ResetBatch();

#ifdef CR_EXT_BINDLESS_TEXS
  static void AddTextureHandle(uint32_t index, uint64_t handle_id);
#endif

  static void DrawQuad(const char* material_name,
                       const TransformRef& transform);
  static void DrawQuad(const MaterialRef& material, Transform& transform);

  static void ResetStats();
  static const Statistics& GetStats();

 private:
  static void Flush();
};

}  // namespace cosair