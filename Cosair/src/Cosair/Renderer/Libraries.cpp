#include "crpch.h"
#include "Libraries.h"

namespace Cosair {

	std::unordered_map<std::string, ShaderRef> ShaderLibrary::s_ShaderStorage;
	std::unordered_map<std::string, MaterialRef> MaterialLibrary::s_MaterialStorage;
	std::unordered_map<std::string, Texture2dRef> Texture2dLibrary::s_Texture2dStorage;

}