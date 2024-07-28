#pragma once

#include <cosair.h>

class MoveGround : public cosair::ScriptableEntity {
 public:
  void OnUpdate(cosair::Timestep ts);
};