#include "move_pillar.h"

#include "../game.h"

void MovePillar::OnUpdate(cosair::Timestep ts) {
  static float move_speed = 0.6f;
  if (Game::GetGameState() == GameState::GameOver) return;
  cosair::Transform& transform = GetComponent<cosair::Transform>();
  if (transform.GetPosX() < -2.5f) Game::SetNewRandomPillarPos();
  transform.SetPosX(transform.GetPosX() - move_speed * ts);
}