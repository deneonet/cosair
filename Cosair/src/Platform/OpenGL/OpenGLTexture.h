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
	#ifdef CR_EXT_BINDLESS_TEXS
		inline virtual uint32_t GetIndex() const override { return m_Index; }
		inline virtual uint64_t GetHandleId() const override { return m_HandleId; }
	#endif
		inline virtual uint32_t GetTextureId() const override { return m_TextureId; }

		inline virtual void Bind(uint32_t slot = 0) const override { glBindTextureUnit(slot, m_TextureId); }
	private:
		std::string m_Path;
		uint32_t m_TextureId;
	};

}