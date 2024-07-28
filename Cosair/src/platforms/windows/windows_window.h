#ifdef CR_PLATFORM_WINDOWS

#pragma once

#include <GLFW/glfw3.h>

#include "cosair/core/window.h"
#include "cosair/renderer/rendering_context.h"

struct GLFWwindow;

namespace cosair {

class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void OnUpdate() override;

  inline unsigned int GetWidth() const override { return data_.width; }
  inline unsigned int GetHeight() const override { return data_.height; }

  inline void* GetNativeWindow() const override { return window_; }

  inline void SetEventCallback(const EventCallbackFn& callback) override {
    data_.event_callback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* window_;
  RenderingContext* rendering_context_;

  struct WindowData {
    std::string title = "";
    unsigned int width = 0;
    unsigned int height = 0;

    bool vsync = true;

    EventCallbackFn event_callback;
  };

  WindowData data_;
};

}  // namespace cosair

#endif