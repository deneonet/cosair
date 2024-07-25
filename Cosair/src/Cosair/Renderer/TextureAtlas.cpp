#include "crpch.h"
#include "TextureAtlas.h"

namespace Cosair {

	void TextureAtlas::AddSubTexture(const char* name, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& size) {
		CR_PROFILE_FUNCTION();

		float textureWidth = m_Texture->GetWidth();
		float textureHeight = m_Texture->GetHeight();
		glm::vec2 min = { (coords.x * cellSize.x) / textureWidth, (coords.y * cellSize.y) / textureHeight };
		glm::vec2 max = { ((coords.x + size.x) * cellSize.x) / textureWidth, ((coords.y + size.y) * cellSize.y) / textureHeight };

		m_SubTextures.insert(std::make_pair(name, TextureCoords {
			min.x, min.y,
			max.x, min.y,
			max.x, max.y,
			min.x, max.y
			}));
	}

}