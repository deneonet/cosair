#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Cosair {

	struct TextureCoords {
		float X1, Y1;
		float X2, Y2;
		float X3, Y3;
		float X4, Y4;
	};

	class TextureAtlas {
	public:
		TextureAtlas(const std::string& path) : m_Texture(Texture2D::Create(path)) { }
		TextureAtlas(const Texture2dRef& texture) : m_Texture(texture) { }

		void AddSubTexture(const char* name, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& size = { 1, 1 });
		inline TextureCoords LoadSubTexture(const char* name) { return m_SubTextures[name]; }

		inline const Texture2dRef& GetTexture() const { return m_Texture; }
	private:
		Texture2dRef m_Texture;
		std::unordered_map<std::string, TextureCoords> m_SubTextures;
	};

	using TextureAtlasRef = Ref<TextureAtlas>;
}