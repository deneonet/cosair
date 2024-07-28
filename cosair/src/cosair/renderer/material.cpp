#include "crpch.h"

#include "material.h"

#include "renderer.h"
#include "renderer_2d.h"

namespace cosair {

ShaderRef Material::texture_shader_;
ShaderRef Material::flat_color_shader_;

bool Material::flush_viewproj_matrix_ = true;

void Material::Init() {
  CR_PROFILE_FUNCTION();

  flat_color_shader_ = Shader::Create("assets/shaders/FlatColor.glsl");
#ifdef CR_EXT_BINDLESS_TEXS
  texture_shader_ = Shader::Create("assets/shaders/Bindless.glsl");
#else
  texture_shader_ = Shader::Create("assets/shaders/Texture.glsl");
#endif
}

float Material::Bind(RendererData2D& data) {
  CR_PROFILE_FUNCTION();

  static uint32_t last_shader_id;
  uint32_t shader_id = shader_->GetId();
  if (last_shader_id != shader_id) {
    shader_->Bind();

#ifndef CR_EXT_BINDLESS_TEXS
    int32_t samplers[data.kMaxTextureSlots];
    for (uint32_t i = 0; i < data.kMaxTextureSlots; i++) samplers[i] = i;
    shader_->SetIntArray("u_Textures", samplers, data.kMaxTextureSlots);
#endif

    last_shader_id = shader_id;
    flush_viewproj_matrix_ = true;
  }

  if (flush_viewproj_matrix_) {
    shader_->SetMatrix4("u_ViewProjection", data.view_proj_matrix);
    flush_viewproj_matrix_ = false;
  }

  if (!shader_vars_.empty()) SetShaderVars();

#ifdef CR_EXT_BINDLESS_TEXS
  return texture_ ? (float)texture_->GetIndex() : 0.0f;
#else
  float texture_index = 0;

  if (texture_) {
    if (data.texture_slots_index >= data.kMaxTextureSlots) {
      Renderer2D::EndScene();
      Renderer2D::ResetBatch();
    }

    for (uint32_t i = 1; i < data.texture_slots_index; i++) {
      if (data.texture_slots[i]->GetId() == texture_->GetId()) {
        texture_index = (float)i;
        break;
      }
    }

    if (texture_index == 0) {
      uint32_t free_texture_index = data.texture_slots_index;
      texture_index = (float)free_texture_index;
      data.texture_slots[free_texture_index] = texture_;
      data.texture_slots_index++;
    }
  }
  return texture_index;
#endif
}

void Material::RemoveTexture() {
  CR_PROFILE_FUNCTION();

  texture_ = nullptr;
  if (shader_->GetId() == texture_shader_->GetId())
    shader_ = flat_color_shader_;
}

void Material::SetTexture(const Texture2dRef& texture) {
  CR_PROFILE_FUNCTION();

  texture_ = texture;
  texture_coords_ = TextureCoords{0, 0, 1, 0, 1, 1, 0, 1};
  if (shader_->GetId() == flat_color_shader_->GetId())
    shader_ = texture_shader_;
}

void Material::SetSubTexture(const char* sub_texture_name) {
  CR_PROFILE_FUNCTION();
  if (!texture_atlas_.get()) {
    CR_CORE_CRITICAL("No Texture-Atlas is set");
    return;
  }

  texture_ = texture_atlas_->GetTexture();
  if (shader_->GetId() == flat_color_shader_->GetId())
    shader_ = texture_shader_;

  texture_coords_ = texture_atlas_->LoadSubTexture(sub_texture_name);
  if (texture_coords_.IsInvalid()) {
    CR_CORE_CRITICAL("Sub-Texture '{0}' not found", sub_texture_name);
  }
}

void Material::SetShaderVars() {
  CR_PROFILE_FUNCTION();

  for (const auto& [name, value] : shader_vars_) {
    std::visit(
        [&](auto&& val) {
          using T = std::decay_t<decltype(val)>;
          if constexpr (std::is_same_v<T, glm::mat4>)
            shader_->SetMatrix4(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::mat3>)
            shader_->SetMatrix3(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::vec4>)
            shader_->SetFloat4(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::vec3>)
            shader_->SetFloat3(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::vec2>)
            shader_->SetFloat2(name.c_str(), val);
          else if constexpr (std::is_same_v<T, float>)
            shader_->SetFloat(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::ivec4>)
            shader_->SetInt4(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::ivec3>)
            shader_->SetInt3(name.c_str(), val);
          else if constexpr (std::is_same_v<T, glm::ivec2>)
            shader_->SetInt2(name.c_str(), val);
          else if constexpr (std::is_same_v<T, int>)
            shader_->SetInt(name.c_str(), val);
          else if constexpr (std::is_same_v<T, int*>)
            shader_->SetIntArray(name.c_str(), val, sizeof(val) / sizeof(int));
        },
        value);
  }
}

}  // namespace cosair