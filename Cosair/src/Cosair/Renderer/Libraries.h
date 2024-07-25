#pragma once

#include "Material.h"
#include "Texture.h"

namespace Cosair {

	class Texture2dLibrary {
	public:
		inline static const Texture2dRef& Load(const char* name) { return s_Texture2dStorage[name]; }
		inline static void Add(const char* name, const Texture2dRef& texture) { s_Texture2dStorage[name] = texture; }
	private:
		static std::unordered_map<std::string, Texture2dRef> s_Texture2dStorage;
	};

	class MaterialLibrary {
	public:
		inline static const MaterialRef& Load(const char* name) { return s_MaterialStorage[name]; }
		inline static void Add(const char* name, const MaterialRef& material) { s_MaterialStorage[name] = material; }
	private:
		static std::unordered_map<std::string, MaterialRef> s_MaterialStorage;
	};

	class ShaderLibrary {
	public:
		inline static const ShaderRef& Load(const char* name) { return s_ShaderStorage[name]; }
		inline static void Add(const char* name, const ShaderRef& shader) { s_ShaderStorage[name] = shader; }
	private:
		static std::unordered_map<std::string, ShaderRef> s_ShaderStorage;
	};

}