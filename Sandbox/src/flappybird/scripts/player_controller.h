#pragma once

#include <cosair.h>

enum class PlayerState { Idle, MovingUp, Falling };

// Player's Jump Power
constexpr float kJumpPower = 0.9f;
// Player's Gravity Power
constexpr float kGravityPower = 2.f;
// Player's Z Moving Up and Falling Down rotation speed
constexpr float kRotationSpeed = 5.f;
// The time between each player animation change
constexpr float kTimeBetweenEachAnim = 100.0f;

class PlayerController : public cosair::ScriptableEntity {
 public:
  void OnCreate();
  void OnUpdate(cosair::Timestep ts);

  void OnCollisionCheck(cosair::Transform& transform);

 private:
  PlayerState player_state_ = PlayerState::Idle;
  cosair::MaterialRef player_material_ = nullptr;

  float velocity_ = 0.0f;
};