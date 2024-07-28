#include "crpch.h"

#include "opengl_context.h"

#include <glad/glad.h>
// clang-format off
#include <GLFW/glfw3.h>

namespace cosair {

OpenGLContext::OpenGLContext(GLFWwindow* window) : handle_(window) {
  CR_CORE_ASSERT(window, "Window is invalid (NULL)");
}

void OpenGLContext::Init() {
  CR_PROFILE_FUNCTION();

  glfwMakeContextCurrent(handle_);
  int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  CR_CORE_ASSERT(success, "Failed to initialize glad");

  CR_CORE_INFO("OpenGL Info:");
  CR_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
  CR_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
  CR_CORE_INFO("  Version: {0}\n", (char*)glGetString(GL_VERSION));

  int version_major;
  int version_minor;
  glGetIntegerv(GL_MAJOR_VERSION, &version_major);
  glGetIntegerv(GL_MINOR_VERSION, &version_minor);

  CR_CORE_ASSERT(
      version_major > 4 || (version_major == 4 && version_minor >= 5),
      "Cosair requires at least OpenGL version 4.5");
}

void OpenGLContext::SwapBuffers() {
  CR_PROFILE_FUNCTION();
  glfwSwapBuffers(handle_);
}

}  // namespace cosair