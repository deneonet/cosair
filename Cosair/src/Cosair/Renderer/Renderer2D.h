#pragma once

#include "Camera.h"
#include "Material.h"
#include "Transform.h"

namespace Cosair {

	struct RendererProps2D {
		uint32_t MaxQuads = 10000;
		uint32_t MaxBindlessTextures = 500000;
	};

	class Renderer2D {
	public:
		static void Init(RendererProps2D props = RendererProps2D());
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void ResetBatch();

	#ifdef CR_EXT_BINDLESS_TEXS
		static void AddTextureHandle(uint32_t index, uint64_t handleId);
	#endif

		static void DrawQuad(const char* materialName, const TransformRef& transform);
		static void DrawQuad(const MaterialRef& material, const TransformRef& transform);

		static void ResetStats();
		static const Statistics& GetStats();
	private:
		static void Flush();
	};

}