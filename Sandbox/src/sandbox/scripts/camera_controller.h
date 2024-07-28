#pragma once

#include <cosair.h>

class CameraController : public cosair::ScriptableEntity {
 public:
  void OnUpdate(cosair::Timestep ts);

 private:
  float camera_move_speed_ = 3.0f;
  float camera_rotation_speed_ = 75.0f;
};