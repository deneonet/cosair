#include "crpch.h"

#include "libraries.h"

namespace cosair {

std::unordered_map<std::string, Texture2dRef>
    Texture2dLibrary::texture_2d_storage_;
std::unordered_map<std::string, ShaderRef> ShaderLibrary::shader_storage_;
std::unordered_map<std::string, MaterialRef> MaterialLibrary::material_storage_;

}  // namespace cosair