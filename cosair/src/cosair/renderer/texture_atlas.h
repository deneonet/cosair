#pragma once

#include <glm/glm.hpp>

#include "texture.h"

namespace cosair {

struct TextureCoords {
  float x1, y1;
  float x2, y2;
  float x3, y3;
  float x4, y4;

  inline bool IsInvalid() const {
    return x1 == 0 && x2 == 0 && x3 == 0 && x4 == 0;
  }
};

class TextureAtlas {
 public:
  TextureAtlas(const std::string& path) : texture_(Texture2D::Create(path)) {}
  TextureAtlas(const Texture2dRef& texture) : texture_(texture) {}

  void AddSubTexture(const char* name, const glm::vec2& coords,
                     const glm::vec2& cell_size,
                     const glm::vec2& size = {1, 1});
  inline const TextureCoords& LoadSubTexture(const char* name) {
    return sub_textures_[name];
  }

  inline const Texture2dRef& GetTexture() const { return texture_; }

 private:
  Texture2dRef texture_;
  std::unordered_map<std::string, TextureCoords> sub_textures_;
};

using TextureAtlasRef = Ref<TextureAtlas>;

}  // namespace cosair