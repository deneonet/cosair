#pragma once

namespace cosair {

class Input {
 public:
  inline static bool IsKeyPressed(uint32_t keycode) {
    return instance_->IsKeyPressed_Impl(keycode);
  }

  inline static bool IsMouseButtonPressed(uint32_t button) {
    return instance_->IsMouseButtonPressed_Impl(button);
  }

  inline static void GetMousePos(double& x_pos, double& y_pos) {
    return instance_->GetMousePos_Impl(x_pos, y_pos);
  }

 protected:
  virtual bool IsKeyPressed_Impl(uint32_t keycode) = 0;
  virtual bool IsMouseButtonPressed_Impl(uint32_t button) = 0;
  virtual void GetMousePos_Impl(double& x_pos, double& y_pos) = 0;

 private:
  static Scope<Input> instance_;
};

}  // namespace cosair