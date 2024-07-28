#include "crpch.h"

#include "application.h"

#include <GLFW/glfw3.h>

#include "cosair/debug/log.h"
#include "cosair/renderer/render_ops.h"

namespace cosair {

Application* Application::instance_ = nullptr;

// TODO: Ability to set window props when creating the application
Application::Application() {
  CR_PROFILE_FUNCTION();
  CR_CORE_ASSERT(!instance_, "Cannot have more than one application");

  instance_ = this;
  window_ = std::unique_ptr<Window>(Window::Create());
  window_->SetEventCallback(CR_BIND_EVENT_FN(OnEvent));

  RenderOps::Init();

  imgui_layer_ = new ImGuiLayer();
  PushOverlay(imgui_layer_);
}

Application::~Application() {
  CR_PROFILE_FUNCTION();
  RenderOps::Shutdown();
}

void Application::OnEvent(Event& event) {
  CR_PROFILE_FUNCTION();

  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(CR_BIND_EVENT_FN(OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(CR_BIND_EVENT_FN(OnWindowResize));

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->OnEvent(event);
    if (event.IsHandled()) break;
  }
}

void Application::Run() {
  CR_PROFILE_FUNCTION();

  if (running_) {
    CR_CORE_ERROR(
        "Tried to run Application twice using 'Application::Run', do not run "
        "the application manually\n");
    return;
  }
  running_ = true;

  while (running_) {
    CR_PROFILE_SCOPE("Application Run Frame");

    // TODO: Make time cross-platform
    float time = (float)glfwGetTime();
    Timestep ts = time - last_frame_time_;
    last_frame_time_ = time;

    RenderOps::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    RenderOps::Clear();

    if (!minimized_) {
      {
        CR_PROFILE_SCOPE("LayerStack OnUpdate");
        for (Layer* layer : layer_stack_) layer->OnUpdate(ts);
      }

#ifdef CR_EXT_IMGUI
      {
        CR_PROFILE_SCOPE("LayerStack OnImGuiRender");

        imgui_layer_->Begin();
        for (Layer* layer : layer_stack_) layer->OnImGuiRender();
        imgui_layer_->End();
      }
#endif
    }

    window_->OnUpdate();
  }
}

void Application::PushLayer(Layer* layer) {
  CR_PROFILE_FUNCTION();
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PopLayer(Layer* layer) {
  CR_PROFILE_FUNCTION();
  layer_stack_.PopLayer(layer);
}

void Application::PushOverlay(Layer* overlay) {
  CR_PROFILE_FUNCTION();
  layer_stack_.PushOverlay(overlay);
  overlay->OnAttach();
}

void Application::PopOverlay(Layer* overlay) {
  CR_PROFILE_FUNCTION();
  layer_stack_.PopOverlay(overlay);
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
  running_ = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  CR_PROFILE_FUNCTION();

  uint32_t width = e.GetWidth();
  uint32_t height = e.GetHeight();
  minimized_ = width == 0 && height == 0;
  window_->SetMinimized(minimized_);
  RenderOps::SetViewport(0, 0, width, height);
  return true;
}

}  // namespace cosair