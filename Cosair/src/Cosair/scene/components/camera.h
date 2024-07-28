#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include "transform.h"

namespace cosair {

// A cosair component
class Camera {
 public:
  Camera() : transform_(glm::vec3(0)){};

  Camera(float left, float right, float bottom, float top, float z_near = -1,
         float z_far = 1)
      : left_(left),
        right_(right),
        bottom_(bottom),
        top_(top),
        z_near_(z_near),
        z_far_(z_far),
        transform_(glm::vec3(0)),
        proj_matrix_(glm::ortho(left, right, bottom, top, z_near, z_far)) {
    transform_.SetForceRecalculatingMatrix(true);
    view_matrix_ = glm::inverse(transform_.GetTransformMatrix());
    viewproj_matrix_ = proj_matrix_ * view_matrix_;
  }

  inline void SetLeft(float left) {
    left_ = left;
    RecalculateProjMatrix();
  }
  inline void SetRight(float right) {
    right_ = right;
    RecalculateProjMatrix();
  }
  inline void SetBottom(float bottom) {
    bottom_ = bottom;
    RecalculateProjMatrix();
  }
  inline void SetTop(float top) {
    top_ = top;
    RecalculateProjMatrix();
  }

  inline void SetZFar(float z_far) {
    z_far_ = z_far;
    RecalculateProjMatrix();
  }
  inline void SetZNear(float z_near) {
    z_near_ = z_near;
    RecalculateProjMatrix();
  }

  inline float GetLeft() const { return left_; }
  inline float GetRight() const { return right_; }
  inline float GetBottom() const { return bottom_; }
  inline float GetTop() const { return top_; }

  inline float GetZFar() const { return z_far_; }
  inline float GetZNear() const { return z_near_; }

  inline Transform& GetTransform() { return transform_; }
  inline const glm::mat4& GetViewMatrix() const { return view_matrix_; }
  inline const glm::mat4& GetProjMatrix() const { return proj_matrix_; }

  inline const glm::mat4& GetViewProjMatrix() {
    if (transform_.HasMatrixChanged()) {
      view_matrix_ = glm::inverse(transform_.GetTransformMatrix());
      viewproj_matrix_ = proj_matrix_ * view_matrix_;
    }
    return viewproj_matrix_;
  }

  inline const glm::mat4& GetCachedViewProjMatrix() const {
    return viewproj_matrix_;
  }

 private:
  void RecalculateProjMatrix();

 private:
  glm::mat4 proj_matrix_;
  glm::mat4 view_matrix_;
  glm::mat4
      viewproj_matrix_;  // Cache of proj_matrix_ and
                         // transform_->GetTransformMatrix() multiplication

  Transform transform_;

  float left_ = -1.6f;
  float right_ = 1.6f;
  float bottom_ = -0.9f;
  float top_ = 0.9f;

  float z_far_ = 1;
  float z_near_ = -1;
};

/* Perspective Camera (commented out for now)
class PerspectiveCamera {
public:
        PerspectiveCamera(float fov = 30.f, float aspectRatio = 0, float zNear =
0.1f, float zFar = 100.f);

        inline void SetRotation(float rotation);
        inline void SetPosition(const glm::vec3& position);

        inline float GetRotation() { return m_Rotation; }
        inline const glm::vec3& GetPosition() { return m_Position; }

        inline void SetFov(float fov);
        inline void SetAspectRatio(float aspectRatio);

        inline float GetFov() { return m_Fov; }
        inline float GetAspectRatio() { return m_AspectRatio; }

        inline void SetZFar(float zFar);
        inline void SetZNear(float zNear);

        inline float GetZFar() { return m_zFar; }
        inline float GetZNear() { return m_zNear; }

        inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
        inline const glm::mat4& GetProjMatrix() { return m_ProjMatrix; }
        inline const glm::mat4& GetViewProjMatrix() { return m_ViewProjMatrix; }
private:
        void RecalculateViewMatrix();
        void RecalculateProjMatrix();

private:
        glm::mat4 m_ProjMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjMatrix; // Cache of m_ProjMatrix and m_ViewMatrix
multiplication

        glm::vec3 m_Position;
        float m_Rotation; // X Rotation, Y and Z are unnecessary

        float m_Fov = 30.f;
        float m_zFar = 100.f;
        float m_zNear = 0.1f;
        float m_AspectRatio = 0.f;
};
*/

}  // namespace cosair