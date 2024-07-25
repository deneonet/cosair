#pragma once

#include "Texture.h"
#include "VertexArray.h"

namespace Cosair {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float     TexIndex;
	};

	struct Statistics {
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

	#ifdef CR_EXT_RENDER_STATS
		inline uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		inline uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
	#else
		inline uint32_t GetTotalIndexCount() const { return 0; }
		inline uint32_t GetTotalVertexCount() const { return 0; }
	#endif
	};

	struct RendererData2D {
		uint32_t MaxQuads;
		uint32_t IndiciesCount;
		uint32_t VerticiesCount;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		QuadVertex* QuadVertexBufferBase = nullptr;

		glm::vec4 QuadVertexPositions[4];

		VertexArrayRef QuadVertexArray;
		VertexBufferRef QuadVertexBuffer;
		glm::mat4 ViewProjectionMatrix { 1 };

	#ifdef CR_EXT_BINDLESS_TEXS
		Scope<uint64_t> TextureHandles;
		uint32_t TextureHandlesSize = 0;
		bool FlushTextureHandles = false;
		ShaderStorageBufferRef ShaderStorageBuffer;
	#else
		// Max actively bind textures per draw call
		// If you want to remove this limit, just enable the 'Bindless Textures' extension and everything is set
		static const uint32_t MaxTextureSlots = 32;

		uint32_t TextureSlotIndex = 0;
		std::array<Texture2dRef, MaxTextureSlots> TextureSlots;
	#endif

		Statistics Stats;

		RendererData2D(uint32_t maxQuads) : MaxQuads(maxQuads), VerticiesCount(maxQuads * 4), IndiciesCount(maxQuads * 6) { }
		RendererData2D() : RendererData2D(10000) { }
	};

	class Renderer {
		// TODO: Add Submit function
	};

}