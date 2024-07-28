#include "camera_controller.h"

void CameraController::OnUpdate(cosair::Timestep ts) {
  cosair::Transform& transform = GetComponent<cosair::Camera>().GetTransform();

  if (cosair::Input::IsKeyPressed(CR_KEY_W))
    transform.SetPosY(transform.GetPosY() + camera_move_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_S))
    transform.SetPosY(transform.GetPosY() - camera_move_speed_ * ts);

  if (cosair::Input::IsKeyPressed(CR_KEY_A))
    transform.SetPosX(transform.GetPosX() - camera_move_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_D))
    transform.SetPosX(transform.GetPosX() + camera_move_speed_ * ts);

  if (cosair::Input::IsKeyPressed(CR_KEY_J))
    transform.SetRotZ(transform.GetRotZ() + camera_rotation_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_K))
    transform.SetRotZ(transform.GetRotZ() - camera_rotation_speed_ * ts);
}