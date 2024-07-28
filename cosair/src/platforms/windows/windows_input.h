#pragma once

#include "cosair/core/input.h"

namespace cosair {

class WindowsInput : public Input {
 protected:
  virtual bool IsKeyPressed_Impl(uint32_t keycode) override;
  virtual bool IsMouseButtonPressed_Impl(uint32_t button) override;
  virtual void GetMousePos_Impl(double& x_pos, double& y_pos) override;
};

}  // namespace cosair