#pragma once

#include <glad/glad.h>

#include "cosair/renderer/texture.h"

namespace cosair {

class OpenGLTexture2D : public Texture2D {
 public:
  OpenGLTexture2D(const std::string& path);
  virtual ~OpenGLTexture2D() override;

  inline virtual uint32_t GetWidth() const override { return width_; }
  inline virtual uint32_t GetHeight() const override { return height_; }
#ifdef CR_EXT_BINDLESS_TEXS
  inline virtual uint32_t GetIndex() const override { return index_; }
  inline virtual uint64_t GetHandleId() const override { return handle_id_; }
#endif
  inline virtual uint32_t GetId() const override { return opengl_id_; }

  inline virtual void Bind(uint32_t slot = 0) const override {
    glBindTextureUnit(slot, opengl_id_);
  }

 private:
  std::string path_;
  uint32_t opengl_id_;  // The OpenGL Identifier of this Texture2D
};

}  // namespace cosair