#pragma once

#include "cosair/renderer/rendering_context.h"

struct GLFWwindow;

namespace cosair {

class OpenGLContext : public RenderingContext {
 public:
  OpenGLContext(GLFWwindow* window);

  virtual void Init() override;
  virtual void SwapBuffers() override;

 private:
  GLFWwindow* handle_;
};

}  // namespace cosair