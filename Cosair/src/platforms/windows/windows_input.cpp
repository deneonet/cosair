#include "crpch.h"

#include "windows_input.h"

#include <GLFW/glfw3.h>

#include "cosair/core/application.h"

namespace cosair {

Scope<Input> Input::instance_ = std::make_unique<WindowsInput>();

bool WindowsInput::IsKeyPressed_Impl(uint32_t keycode) {
  GLFWwindow* window =
      (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
  int state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressed_Impl(uint32_t button) {
  GLFWwindow* window =
      (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
  int state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

void WindowsInput::GetMousePos_Impl(double& x_pos, double& y_pos) {
  GLFWwindow* window =
      (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
  glfwGetCursorPos(window, &x_pos, &y_pos);
}

}  // namespace cosair