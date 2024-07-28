#include "crpch.h"

#include "opengl_texture.h"

#ifdef CR_EXT_BINDLESS_TEXS
#include "cosair/renderer/renderer_2d.h"
#endif

#include <stb_image.h>

namespace cosair {

#ifdef CR_EXT_BINDLESS_TEXS
uint32_t OpenGLTexture2D::index_offset_ = 0;
#endif

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  CR_PROFILE_FUNCTION();

  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);

  stbi_uc* data = nullptr;
  CR_PROFILE_SCOPE(
      "stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
  data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    CR_PROFILE_SCOPE(
        "stbi_load: fallback image - OpenGLTexture2D::OpenGLTexture2D(const "
        "std::string&)");
    data = stbi_load("assets/engine/TextureFallback.png", &width, &height,
                     &channels, 0);
    CR_CORE_ERROR("Image '{0}' not found", path);
    CR_CORE_ERROR("Using fallback image\n");
  }

  CR_CORE_ASSERT(channels == 3 || channels == 4,
                 "Image '{0}' is not RGB, nor RGBA\n", path);

  width_ = width;
  height_ = height;

  glCreateTextures(GL_TEXTURE_2D, 1, &opengl_id_);
  glTextureStorage2D(opengl_id_, 1, channels == 3 ? GL_RGB8 : GL_RGBA8, width_,
                     height_);

  glTextureParameteri(opengl_id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(opengl_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureSubImage2D(opengl_id_, 0, 0, 0, width_, height_,
                      channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);

#ifdef CR_EXT_BINDLESS_TEXS
  index_ = index_offset_;
  handle_id_ = glGetTextureHandleARB(opengl_id_);
  glMakeTextureHandleResidentARB(handle_id_);
  index_offset_++;

  Renderer2D::AddTextureHandle(index_, handle_id_);
#endif
}

OpenGLTexture2D::~OpenGLTexture2D() {
  CR_PROFILE_FUNCTION();
  glDeleteTextures(1, &opengl_id_);
}

}  // namespace cosair