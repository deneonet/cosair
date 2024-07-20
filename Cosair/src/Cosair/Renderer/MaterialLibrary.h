#pragma once

#include "Material.h"

namespace Cosair {

	class MaterialLibrary {
	public:
		inline static const MaterialRef& Load(const char* name) { return s_MaterialStorage[name]; }
		inline static void Add(const char* name, const MaterialRef& material) { s_MaterialStorage[name] = material; }
	private:
		static std::unordered_map<std::string, MaterialRef> s_MaterialStorage;
	};

}