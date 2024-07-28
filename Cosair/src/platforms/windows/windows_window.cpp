#include "crpch.h"

#include "windows_window.h"

#include "cosair/events/key_event.h"
#include "cosair/events/mouse_event.h"
#include "cosair/events/window_event.h"
#include "cosair/renderer/renderer_api.h"
#include "platforms/opengl/opengl_context.h"

namespace cosair {

static uint8_t glfw_window_count_ = 0;

static void GlfwErrorCallback(int error, const char* description) {
  CR_CORE_ERROR("GLFW Error \"{0}\": {1}", error, description);
}

Window* Window::Create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
  CR_PROFILE_FUNCTION();
  Init(props);
}

WindowsWindow::~WindowsWindow() {
  CR_PROFILE_FUNCTION();

  glfw_window_count_--;
  if (glfw_window_count_ == 0) {
    CR_CORE_INFO("Terminating GLFW\n");
    Shutdown();
  }
}

void WindowsWindow::Init(const WindowProps& props) {
  CR_PROFILE_FUNCTION();

  data_.title = props.title;
  data_.width = props.width;
  data_.height = props.height;

  if (glfw_window_count_ == 0) {
    CR_CORE_INFO("Initializing GLFW\n");

    int success = glfwInit();
    CR_CORE_ASSERT(success, "Could not initialize GLFW");
    glfwSetErrorCallback(GlfwErrorCallback);
  }

#ifdef CR_DEBUG
  if (RendererApi::GetApi() == RendererApi::Api::kOpenGL)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

  CR_CORE_INFO("Creating window {0} ({1}, {2})\n", props.title, props.width,
               props.height);
  window_ = glfwCreateWindow(props.width, props.height, data_.title.c_str(),
                             nullptr, nullptr);
  ++glfw_window_count_;

  rendering_context_ = new OpenGLContext(window_);
  rendering_context_->Init();

  glfwSetWindowUserPointer(window_, &data_);

  SetVSync(true);

  glfwSetWindowSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.event_callback(event);
      });

  glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.event_callback(event);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.event_callback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        data.event_callback(event);
        break;
      }
    }
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.event_callback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.event_callback(event);
            break;
          }
        }
      });

  glfwSetScrollCallback(
      window_, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event(xOffset, yOffset);
        data.event_callback(event);
      });

  glfwSetCursorPosCallback(
      window_, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event(xPos, yPos);
        data.event_callback(event);
      });
}

void WindowsWindow::Shutdown() {
  CR_PROFILE_FUNCTION();

  glfwDestroyWindow(window_);
  glfwTerminate();
}

void WindowsWindow::OnUpdate() {
  CR_PROFILE_FUNCTION();

  glfwPollEvents();
  rendering_context_->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
  CR_PROFILE_FUNCTION();

  glfwSwapInterval(enabled ? 1 : 0);
  data_.vsync = enabled;
}

bool WindowsWindow::IsVSync() const { return data_.vsync; }

}  // namespace cosair