#pragma once

#include "cosair/core/layer.h"

namespace cosair {

class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnImGuiRender() override;

  void Begin();
  void End();

 private:
  static bool attached_;
};

}  // namespace cosair