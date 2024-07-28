#pragma once

#include <cosair.h>

class AnyaController : public cosair::ScriptableEntity {
 public:
  void OnUpdate(cosair::Timestep ts);

 private:
  float anya_move_speed_ = 3.0f;
  float anya_rotation_speed_ = 75.0f;
};