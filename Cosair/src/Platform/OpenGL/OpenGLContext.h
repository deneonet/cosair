#pragma once

#include "Cosair/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Cosair {

	class OpenGLContext : public RenderingContext {
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_Handle;
	};

}