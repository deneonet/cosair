#pragma once

#include "bindless_textures_manager.h"
#include "texture.h"
#include "vertex_array.h"

namespace cosair {

struct QuadVertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 tex_coord;
  float tex_index;
};

struct Statistics {
  uint32_t draw_calls = 0;
  uint32_t quad_count = 0;

#ifdef CR_EXT_RENDER_STATS
  inline uint32_t GetTotalIndexCount() const { return quad_count * 6; }
  inline uint32_t GetTotalVertexCount() const { return quad_count * 4; }
#else
  inline uint32_t GetTotalIndexCount() const { return 0; }
  inline uint32_t GetTotalVertexCount() const { return 0; }
#endif
};

struct RendererData2D {
  size_t max_quads = 0;
  size_t indicies_count = 0;
  size_t verticies_count = 0;

  size_t quad_index_count = 0;
  QuadVertex* quad_vertex_buffer_ptr = nullptr;
  QuadVertex* quad_vertex_buffer_base = nullptr;

  glm::vec4 quad_vertex_positions[4]{{-0.5f, -0.5f, 0.0f, 1.0f},
                                     {0.5f, -0.5f, 0.0f, 1.0f},
                                     {0.5f, 0.5f, 0.0f, 1.0f},
                                     {-0.5f, 0.5f, 0.0f, 1.0f}};

  VertexArrayRef quad_vertex_array;
  VertexBufferRef quad_vertex_buffer;
  glm::mat4 view_proj_matrix{1};

#ifdef CR_EXT_BINDLESS_TEXS
  BindlessTexturesManagerRef bindless_textures_manager = nullptr;
#else
  // Max actively bind textures per draw call
  static const uint32_t kMaxTextureSlots = 32;

  uint32_t texture_slots_index = 0;
  std::array<Texture2dRef, kMaxTextureSlots> texture_slots;
#endif

  Statistics stats;

  RendererData2D() = default;
  RendererData2D(size_t max_quads
#ifdef CR_EXT_BINDLESS_TEXS
                 ,
                 size_t max_bindless_textures
#endif
                 )
      : max_quads(max_quads),
        verticies_count(max_quads * 4),
        indicies_count(max_quads * 6)
#ifdef CR_EXT_BINDLESS_TEXS
        ,
        bindless_textures_manager(
            CreateRef<BindlessTexturesManager>(max_bindless_textures))
#endif
  {
  }
};  // namespace cosair

class Renderer {
  // TODO: Add Submit function
};

}  // namespace cosair