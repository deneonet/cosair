#include "crpch.h"
#include "OpenGLRendererAPI.h"

namespace Cosair {

#ifdef CR_DEBUG
	static void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         CR_CORE_CRITICAL(message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       CR_CORE_ERROR(message); break;
		case GL_DEBUG_SEVERITY_LOW:          CR_CORE_WARN(message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: CR_CORE_TRACE(message); break;
		}

		CR_CORE_DERROR("OpenGL error found!");
	}
#endif

	void OpenGLRendererAPI::Init() {
		CR_PROFILE_FUNCTION();

	#ifdef CR_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const VertexArrayRef& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawIndexed(const VertexArrayRef& vertexArray, uint32_t indiciesCount) {
		glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, nullptr);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

}