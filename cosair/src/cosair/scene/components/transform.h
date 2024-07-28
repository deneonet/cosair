#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "cosair/debug/instrumentor.h"

namespace cosair {

// A cosair component
class Transform {
 public:
  Transform(const Transform& transform)
      : position_(transform.GetPos()),
        rotation_(transform.GetRot()),
        scale_(transform.GetScale()) {
    CR_PROFILE_FUNCTION();
    RecalculateTransformMatrix();
  }

  Transform(const glm::vec3& position)
      : position_(position), rotation_(glm::vec3(0)), scale_(glm::vec3(1)) {
    CR_PROFILE_FUNCTION();
    RecalculateTransformMatrix();
  }

  Transform(const glm::vec3& position, const glm::vec3& rotation)
      : position_(position), rotation_(rotation), scale_(glm::vec3(1)) {
    CR_PROFILE_FUNCTION();
    RecalculateTransformMatrix();
  }

  Transform(const glm::vec3& position, const glm::vec3& rotation,
            const glm::vec3& scale)
      : position_(position), rotation_(rotation), scale_(scale) {
    CR_PROFILE_FUNCTION();
    RecalculateTransformMatrix();
  }

  inline void Set(const glm::vec3& position, const glm::vec3& rotation,
                           const glm::vec3& scale) {
    position_ = position;
    rotation_ = rotation;
    scale_ = scale;
    RecalculateTransformMatrix();
  }

  inline void SetScale(const glm::vec3& scale) {
    scale_ = scale;
    RecalculateTransformMatrix();
  }

  inline void SetPos(const glm::vec3& position) {
    position_ = position;
    RecalculateTransformMatrix();
  }

  inline void SetRot(const glm::vec3& rotation) {
    rotation_ = rotation;
    RecalculateTransformMatrix();
  }

  inline bool HasScale() const {
    return scale_.x != 0 || scale_.y != 0 || scale_.z != 0;
  }
  inline bool HasRotation() const {
    return rotation_.x != 0 || rotation_.y != 0 || rotation_.z != 0;
  }
  inline bool HasPosition() const {
    return position_.x != 0 || position_.y != 0 || position_.z != 0;
  }

  inline const glm::vec3& GetScale() const { return scale_; }
  inline const glm::vec3& GetPos() const { return position_; }
  inline const glm::vec3& GetRot() const { return rotation_; }

  inline void SetScaleX(float x) {
    scale_.x = x;
    RecalculateTransformMatrix();
  }
  inline void SetScaleY(float y) {
    scale_.y = y;
    RecalculateTransformMatrix();
  }
  inline void SetScaleZ(float z) {
    scale_.z = z;
    RecalculateTransformMatrix();
  }

  inline void SetPosX(float x) {
    position_.x = x;
    RecalculateTransformMatrix();
  }
  inline void SetPosY(float y) {
    position_.y = y;
    RecalculateTransformMatrix();
  }
  inline void SetPosZ(float z) {
    position_.z = z;
    RecalculateTransformMatrix();
  }

  inline void SetRotX(float x) {
    rotation_.x = x;
    RecalculateTransformMatrix();
  }
  inline void SetRotY(float y) {
    rotation_.y = y;
    RecalculateTransformMatrix();
  }
  inline void SetRotZ(float z) {
    rotation_.z = z;
    RecalculateTransformMatrix();
  }

  inline const float GetScaleX() const { return scale_.x; }
  inline const float GetScaleY() const { return scale_.y; }
  inline const float GetScaleZ() const { return scale_.z; }

  inline const float GetPosX() const { return position_.x; }
  inline const float GetPosY() const { return position_.y; }
  inline const float GetPosZ() const { return position_.z; }

  inline const float GetRotX() const { return rotation_.x; }
  inline const float GetRotY() const { return rotation_.y; }
  inline const float GetRotZ() const { return rotation_.z; }

  // TODO: Add operator for glm::mat4, maybe?

  // The transform matrix is updated only if rotation (x, y, z) is non-zero
  // and there were changes in rotation, position, or scale.
  inline const glm::mat4& GetTransformMatrix() {
    transform_matrix_changed_ = false;
    return transform_matrix_;
  }

  // Toggles whether the transform matrix should be recalculated on position or
  // scale changes, even when rotation (x, y, z) is zero.
  inline void SetForceRecalculatingMatrix(bool enabled) {
    force_recalculating_matrix_ = enabled;
    RecalculateTransformMatrix();
  }

  // Indicates whether the transform matrix has changed. Once called, it returns
  // true if the matrix has changed since the last
  // 'Transform::GetTransformMatrix' call; further calls will return true until
  // 'Transform::GetTransformMatrix' is called.
  inline bool HasMatrixChanged() const { return transform_matrix_changed_; }

 private:
  void RecalculateTransformMatrix();

 private:
  bool transform_matrix_changed_ = false;
  bool force_recalculating_matrix_ = false;

  glm::vec3 scale_;
  glm::vec3 position_;
  glm::vec3 rotation_;

  glm::mat4 transform_matrix_;  // Cache calculation of the position, rotation
                                // and scale, only updated, if the rotation
                                // (x, y, z) is not 0
};

using TransformRef = Ref<Transform>;

}  // namespace cosair
