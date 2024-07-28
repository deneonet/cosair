#pragma once

#include "cosair/core.h"
#include "cosair/events/event.h"
#include "timestep.h"

namespace cosair {

class Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}
  virtual void OnUpdate(Timestep ts) {}

  inline const std::string& GetName() const { return debug_name_; }

 protected:
  std::string debug_name_;
};

}  // namespace cosair