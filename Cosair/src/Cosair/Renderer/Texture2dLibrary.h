#pragma once

#include "Texture.h"

namespace Cosair {

	class Texture2dLibrary {
	public:
		inline static const Texture2dRef& Load(const char* name) { return s_Texture2dStorage[name]; }
		inline static void Add(const char* name, const Texture2dRef& texture) { s_Texture2dStorage[name] = texture; }
	private:
		static std::unordered_map<std::string, Texture2dRef> s_Texture2dStorage;
	};

}