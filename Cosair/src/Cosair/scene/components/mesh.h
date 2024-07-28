#pragma once

#include "cosair/renderer/material.h"
#include "cosair/renderer/renderer_2d.h"

namespace cosair {

enum class MeshType {
  kQuad,
};

// A cosair component
class Mesh {
 public:
  Mesh(const MaterialRef& material, MeshType type = MeshType::kQuad)
      : material_(material), type_(type) {}

  inline void Draw(Transform& transform) const {
    // TODO: Revisit and improve this, maybe?
    switch (type_) {
      case MeshType::kQuad:
        Renderer2D::DrawQuad(material_, transform);
    }
  };

  inline void SetMaterial(const MaterialRef& material) {
    material_ = material;
  };

  inline MaterialRef& GetMaterial() { return material_; };

 private:
  MeshType type_ = MeshType::kQuad;
  MaterialRef material_ = nullptr;
};

}  // namespace cosair