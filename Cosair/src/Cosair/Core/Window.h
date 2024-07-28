#pragma once

#include "cosair/core.h"
#include "cosair/events/event.h"

namespace cosair {

struct WindowProps {
  std::string title;
  unsigned int width;
  unsigned int height;

  WindowProps(const std::string& title = "Cosair Engine",
              unsigned int width = 1280, unsigned int height = 720)
      : title(title), width(width), height(height) {}
};

class Window {
 public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window() = default;

  virtual void OnUpdate() = 0;

  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;

  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

  virtual bool IsVSync() const = 0;
  virtual void SetVSync(bool enabled) = 0;

  inline bool IsMinimized() const { return minimized_; }
  inline void SetMinimized(bool minimized) { minimized_ = minimized; }

  virtual void* GetNativeWindow() const = 0;

  static Window* Create(const WindowProps& props = WindowProps());

 protected:
  bool minimized_ = false;
};

}  // namespace cosair