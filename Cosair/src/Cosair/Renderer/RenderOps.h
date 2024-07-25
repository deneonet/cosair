#pragma once

#include "Material.h"
#include "Renderer2D.h"
#include "RendererAPI.h"

namespace Cosair {

	class RenderOps {
	public:
		inline static void Init() {
			s_RendererAPI->Init();

			Material::Init();
			Renderer2D::Init();
		}

		inline static void Shutdown() {
			Renderer2D::Shutdown();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		// Draws the entire vertex array
		inline static void DrawIndexed(const VertexArrayRef& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		// Draws the count of indicies provided using the vertex array
		inline static void DrawIndexed(const VertexArrayRef& vertexArray, uint32_t indiciesCount) {
			s_RendererAPI->DrawIndexed(vertexArray, indiciesCount);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}