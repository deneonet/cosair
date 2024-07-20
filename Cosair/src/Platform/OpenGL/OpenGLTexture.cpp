#include "crpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Cosair {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
#ifndef CR_DIST
		// TODO: Make a cross-platform file utility class
		std::ifstream f(path);
		if (!f.good())
			CR_CORE_ERROR("Image '{0}' not found", path);
#endif

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data) {
			data = stbi_load("assets/engine/TextureFallback.png", &width, &height, &channels, 0);
			CR_CORE_ERROR("Using fallback image\n");
		}

		CR_CORE_ASSERT(channels == 3 || channels == 4, "Image '{0}' is not RGB, nor RGBA\n", path)

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, 
			channels == 3 ? GL_RGB8 : GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, 
			channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_TextureID);
	}

}