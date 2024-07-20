#pragma once

#include "Cosair/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Cosair {

#ifdef CR_DEBUG
	static void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         CR_CORE_CRITICAL(message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       CR_CORE_ERROR(message); break;
		case GL_DEBUG_SEVERITY_LOW:          CR_CORE_WARN(message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: CR_CORE_TRACE(message); break;
		}

		CR_CORE_DERROR("OpenGL error found!");
	}
#endif
	
	class OpenGLRendererAPI : public RendererAPI {
		virtual void Init() override {
#ifdef CR_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const VertexArrayRef& vertexArray) override;
	};

}