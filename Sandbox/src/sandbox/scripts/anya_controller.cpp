#include "anya_controller.h"

void AnyaController::OnUpdate(cosair::Timestep ts) {
  cosair::Transform& transform = GetComponent<cosair::Transform>();

  /*
  if (cosair::Input::IsKeyPressed(CR_KEY_W))
    transform.SetPosY(transform.GetPosY() + anya_move_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_S))
    transform.SetPosY(transform.GetPosY() - anya_move_speed_ * ts);
  */

  if (cosair::Input::IsKeyPressed(CR_KEY_O))
    transform.SetPosX(transform.GetPosX() - anya_move_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_P))
    transform.SetPosX(transform.GetPosX() + anya_move_speed_ * ts);

  if (cosair::Input::IsKeyPressed(CR_KEY_U))
    transform.SetRotZ(transform.GetRotZ() + anya_rotation_speed_ * ts);
  else if (cosair::Input::IsKeyPressed(CR_KEY_I))
    transform.SetRotZ(transform.GetRotZ() - anya_rotation_speed_ * ts);
}