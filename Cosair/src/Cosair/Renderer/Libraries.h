#pragma once

#include "material.h"
#include "texture.h"

namespace cosair {

class Texture2dLibrary {
 public:
  inline static const Texture2dRef& Load(const char* name) {
    return texture_2d_storage_[name];
  }
  inline static void Add(const char* name, const Texture2dRef& texture) {
    texture_2d_storage_[name] = texture;
  }

 private:
  static std::unordered_map<std::string, Texture2dRef> texture_2d_storage_;
};

class MaterialLibrary {
 public:
  inline static const MaterialRef& Load(const char* name) {
    return material_storage_[name];
  }
  inline static void Add(const char* name, const MaterialRef& material) {
    material_storage_[name] = material;
  }

 private:
  static std::unordered_map<std::string, MaterialRef> material_storage_;
};

class ShaderLibrary {
 public:
  inline static const ShaderRef& Load(const char* name) {
    return shader_storage_[name];
  }
  inline static void Add(const char* name, const ShaderRef& shader) {
    shader_storage_[name] = shader;
  }

 private:
  static std::unordered_map<std::string, ShaderRef> shader_storage_;
};

}  // namespace cosair