#pragma once

#include "cosair/core.h"
#include "cosair/debug/imgui_layer.h"
#include "cosair/events/event.h"
#include "cosair/events/window_event.h"
#include "layer.h"
#include "layer_stack.h"
#include "timestep.h"
#include "window.h"

#define CR_BIND_EVENT_FN(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

int main(int argc, char** argv);

namespace cosair {

class Application {
 public:
  Application();
  virtual ~Application();

  void OnEvent(Event& event);

  void PushLayer(Layer* layer);
  void PopLayer(Layer* layer);
  void PushOverlay(Layer* layer);
  void PopOverlay(Layer* layer);

  inline Window& GetWindow() { return *window_; }
  inline static Application& Get() { return *instance_; }

 private:
  void Run();
  bool OnWindowClose(WindowCloseEvent& e);
  bool OnWindowResize(WindowResizeEvent& e);

 private:
  LayerStack layer_stack_;
  ImGuiLayer* imgui_layer_;

  float last_frame_time_ = 0;

  bool running_ = false;
  bool minimized_ = false;
  std::unique_ptr<Window> window_;

 private:
  static Application* instance_;
#ifdef CR_PLATFORM_WINDOWS
  friend int ::main(int argc, char** argv);
#endif
};

Application* CreateApplication();

}  // namespace cosair
