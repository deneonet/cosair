#include "move_ground.h"

#include "../game.h"

void MoveGround::OnUpdate(cosair::Timestep ts) {
  static float move_speed = 0.6f;
  if (Game::GetGameState() == GameState::GameOver) return;
  cosair::Transform& transform = GetComponent<cosair::Transform>();
  if (transform.GetPosX() < -2.9f) transform.SetPosX(2.8f);
  transform.SetPosX(transform.GetPosX() - move_speed * ts);
}