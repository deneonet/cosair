#pragma once

#include "Cosair/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Cosair {

	class OpenGLRendererAPI : public RendererAPI {
		virtual void Init() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const VertexArrayRef& vertexArray) override;
		virtual void DrawIndexed(const VertexArrayRef& vertexArray, uint32_t indiciesCount) override;
	};

}