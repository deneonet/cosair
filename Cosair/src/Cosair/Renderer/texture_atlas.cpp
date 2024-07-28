#include "crpch.h"

#include "texture_atlas.h"

namespace cosair {

void TextureAtlas::AddSubTexture(const char* name, const glm::vec2& coords,
                                 const glm::vec2& cell_size,
                                 const glm::vec2& size) {
  CR_PROFILE_FUNCTION();

  uint32_t texture_width = texture_->GetWidth();
  uint32_t texture_height = texture_->GetHeight();

  glm::vec2 min = {(coords.x * cell_size.x) / texture_width,
                   (coords.y * cell_size.y) / texture_height};
  glm::vec2 max = {((coords.x + size.x) * cell_size.x) / texture_width,
                   ((coords.y + size.y) * cell_size.y) / texture_height};

  sub_textures_.insert(std::make_pair(
      name,
      TextureCoords{min.x, min.y, max.x, min.y, max.x, max.y, min.x, max.y}));
}

}  // namespace cosair