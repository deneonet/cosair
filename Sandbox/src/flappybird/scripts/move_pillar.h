#pragma once

#include <cosair.h>

class MovePillar : public cosair::ScriptableEntity {
 public:
  void OnUpdate(cosair::Timestep ts);
};