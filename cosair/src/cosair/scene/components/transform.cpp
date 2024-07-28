#include "crpch.h"

#include "transform.h"

namespace cosair {

void Transform::RecalculateTransformMatrix() {
  CR_PROFILE_FUNCTION();
  if (!force_recalculating_matrix_ && !HasRotation()) return;

  transform_matrix_ = glm::translate(glm::mat4(1), position_) *
                      glm::scale(glm::mat4(1), scale_);
  if (rotation_.x != 0)
    transform_matrix_ *= glm::rotate(glm::mat4(1), glm::radians(rotation_.x),
                                     glm::vec3(1, 0, 0));
  if (rotation_.y != 0)
    transform_matrix_ *= glm::rotate(glm::mat4(1), glm::radians(rotation_.y),
                                     glm::vec3(0, 1, 0));
  if (rotation_.z != 0)
    transform_matrix_ *= glm::rotate(glm::mat4(1), glm::radians(rotation_.z),
                                     glm::vec3(0, 0, 1));

  transform_matrix_changed_ = true;
}

}  // namespace cosair
