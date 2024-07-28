#pragma once

namespace cosair {

class RenderingContext {
 public:
  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;
};

}  // namespace cosair