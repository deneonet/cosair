#include "crpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cosair {

	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Handle(window) {
		CR_CORE_ASSERT(window, "Window is null");
	}

	void OpenGLContext::Init() {
		CR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_Handle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CR_CORE_ASSERT(success, "Failed to initialize glad");

		CR_CORE_INFO("OpenGL Info:");
		CR_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		CR_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		CR_CORE_INFO("  Version: {0}\n", (char*)glGetString(GL_VERSION));

		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Cosair requires at least OpenGL version 4.5");
	}

	void OpenGLContext::SwapBuffers() {
		CR_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Handle);
	}

}