#pragma once

#include <variant>

#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "texture_atlas.h"

namespace cosair {

class Material {
 public:
  Material(const ShaderRef& shader = flat_color_shader_) : shader_(shader) {}

  static void Init();
  static void FlushViewProjMatrix() { flush_viewproj_matrix_ = true; }

  float Bind(RendererData2D& data);

  void RemoveTexture();
  void SetTexture(const Texture2dRef& texture);
  inline const Texture2dRef& GetTexture() const { return texture_; }

  // To remove the Sub-Texture, use 'Material::RemoveTexture()'
  void SetSubTexture(const char* sub_texture_name);
  inline void SetTextureAtlas(const TextureAtlasRef& texture_atlas) {
    texture_atlas_ = texture_atlas;
  }

  inline const TextureCoords& GetTextureCoords() const {
    return texture_coords_;
  }

  void SetColor(const glm::vec4 color) { color_ = color; }
  inline const glm::vec4& GetColor() const { return color_; }

  inline const ShaderRef& GetShader() const { return shader_; }

  template <typename T>
  inline void Set(const std::string& name, const T& value) {
    static_assert(
        std::is_same_v<T, glm::mat4> || std::is_same_v<T, glm::mat3> ||
            std::is_same_v<T, glm::vec4> || std::is_same_v<T, glm::vec3> ||
            std::is_same_v<T, glm::vec2> || std::is_same_v<T, float> ||
            std::is_same_v<T, glm::ivec4> || std::is_same_v<T, glm::ivec3> ||
            std::is_same_v<T, glm::ivec2> || std::is_same_v<T, int> ||
            std::is_same_v<T, int*>,
        "Invalid Type, supported types are: glm::mat4, glm::mat3, glm::vec4, "
        "glm::vec3, glm::vec2, float, glm::ivec4, glm::ivec3, glm::ivec2, int "
        "and int*");
    shader_vars_[name] = value;
  }

 private:
  void SetShaderVars();

 private:
  static ShaderRef texture_shader_;
  static ShaderRef flat_color_shader_;

  static bool flush_viewproj_matrix_;

 private:
  ShaderRef shader_;

  glm::vec4 color_{1};
  Texture2dRef texture_ = nullptr;
  TextureCoords texture_coords_{};
  TextureAtlasRef texture_atlas_ = nullptr;

  using ShaderVarValue =
      std::variant<glm::mat4, glm::mat3, glm::vec4, glm::vec3, glm::vec2, float,
                   glm::ivec4, glm::ivec3, glm::ivec2, int>;
  std::unordered_map<std::string, ShaderVarValue> shader_vars_;
};

using MaterialRef = Ref<Material>;

}  // namespace cosair