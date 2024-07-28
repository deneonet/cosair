#include "crpch.h"

#ifdef CR_EXT_BINDLESS_TEXS

#include "bindless_textures_manager.h"

namespace cosair {

void BindlessTexturesManager::AddTextureHandle(uint32_t index,
                                               uint64_t handle_id) {
  CR_PROFILE_FUNCTION();
  if (index >= max_bindless_textures_) {
    CR_CORE_CRITICAL("Max bindless textures exceeded");
    return;
  }

  texture_handles_[index] = handle_id;
  texture_handles_size_++;
  flush_texture_handles_ = true;
}

void BindlessTexturesManager::BindAndFlushTextureHandles() {
  CR_PROFILE_FUNCTION();

  shds_buffer_->BindBase(0);
  if (flush_texture_handles_) {
    shds_buffer_->SetData(texture_handles_,
                          texture_handles_size_ * sizeof(uint64_t));
    flush_texture_handles_ = false;
  }
}

}  // namespace cosair

#endif