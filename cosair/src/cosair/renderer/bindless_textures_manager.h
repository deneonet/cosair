#pragma once

#ifdef CR_EXT_BINDLESS_TEXS

#include "buffer.h"

namespace cosair {

class BindlessTexturesManager {
 public:
  BindlessTexturesManager(size_t max_bindless_textures)
      : max_bindless_textures_(max_bindless_textures) {
    texture_handles_ = new uint64_t[max_bindless_textures];
    shds_buffer_ = ShdSBuffer::Create(max_bindless_textures);
  }

  ~BindlessTexturesManager() { delete[] texture_handles_; }

  void BindAndFlushTextureHandles();
  void AddTextureHandle(uint32_t index, uint64_t handle_id);

 private:
  uint64_t* texture_handles_;
  ShdSBufferRef shds_buffer_;
  size_t texture_handles_size_ = 0;
  size_t max_bindless_textures_ = 0;
  bool flush_texture_handles_ = false;
};

using BindlessTexturesManagerRef = Ref<BindlessTexturesManager>;

}  // namespace cosair

#endif