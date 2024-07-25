#include "crpch.h"
#include "OpenGLTexture.h"

#ifdef CR_EXT_BINDLESS_TEXS
#include "Cosair/Renderer/Renderer2D.h"
#endif

#include <stb_image.h>

namespace Cosair {

#ifdef CR_EXT_BINDLESS_TEXS
	uint32_t OpenGLTexture2D::m_IndexOffset = 0;
#endif

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
		CR_PROFILE_FUNCTION();

	#ifndef CR_DIST
		// TODO: Make a cross-platform file utility class
		std::ifstream f(path);
		if (!f.good())
			CR_CORE_ERROR("Image '{0}' not found", path);
	#endif

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			CR_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		if (!data) {
			CR_PROFILE_SCOPE("stbi_load: fallback image - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load("assets/engine/TextureFallback.png", &width, &height, &channels, 0);
			CR_CORE_ERROR("Using fallback image\n");
		}

		CR_CORE_ASSERT(channels == 3 || channels == 4, "Image '{0}' is not RGB, nor RGBA\n", path);

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
		glTextureStorage2D(m_TextureId, 1,
			channels == 3 ? GL_RGB8 : GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height,
			channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

	#ifdef CR_EXT_BINDLESS_TEXS
		m_Index = m_IndexOffset;
		m_HandleId = glGetTextureHandleARB(m_TextureId);
		glMakeTextureHandleResidentARB(m_HandleId);
		m_IndexOffset++;

		Renderer2D::AddTextureHandle(m_Index, m_HandleId);
	#endif
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		CR_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_TextureId);
	}

}