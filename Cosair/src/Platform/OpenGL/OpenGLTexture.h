#pragma once

#include "Cosair/Renderer/Texture.h"

#include <glad/glad.h>

namespace Cosair {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D() override;

		inline virtual uint32_t GetWidth() const override { return m_Width; }
		inline virtual uint32_t GetHeight() const override { return m_Height; }

		inline virtual void Bind(uint32_t slot = 0) const override { glBindTextureUnit(slot, m_TextureID); }
	private:
		std::string m_Path;
		uint32_t m_TextureID;
	};

}