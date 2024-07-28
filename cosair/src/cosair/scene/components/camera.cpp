#include "crpch.h"

#include "camera.h"

namespace cosair {

void Camera::RecalculateProjMatrix() {
  view_matrix_ = glm::inverse(transform_.GetTransformMatrix());
  proj_matrix_ = glm::ortho(left_, right_, bottom_, top_, z_near_, z_far_);
  viewproj_matrix_ = proj_matrix_ * view_matrix_;
}

/* Perspective Camera Implementation (commented out for now)
PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear,
float zFar) : m_Fov(fov), m_AspectRatio(aspectRatio), m_zNear(zNear),
m_zFar(zFar) { if (aspectRatio == 0.f) { Window& window =
Application::Get().GetWindow(); aspectRatio = (float)window.GetWidth() /
window.GetHeight();
        }
        RecalculateProjMatrix();
}

void PerspectiveCamera::SetRotation(float rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
}

void PerspectiveCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
}

void PerspectiveCamera::SetFov(float fov) {
        m_Fov = fov;
        RecalculateProjMatrix();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio) {
        m_AspectRatio = aspectRatio;
        RecalculateProjMatrix();
}

void PerspectiveCamera::SetZFar(float zFar) {
        m_zFar = zFar;
        RecalculateProjMatrix();
}

void PerspectiveCamera::SetZNear(float zNear) {
        m_zNear = zNear;
        RecalculateProjMatrix();
}

void PerspectiveCamera::RecalculateViewMatrix() {
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
        m_ViewMatrix = glm::rotate(m_ViewMatrix, m_Rotation, glm::vec3(-1.0f,
0.0f, 0.0f)); m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateProjMatrix() {
        m_ProjMatrix = glm::perspective(m_Fov, m_AspectRatio, m_zNear, m_zFar);
        m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}
*/

}  // namespace cosair