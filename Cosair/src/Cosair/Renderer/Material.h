#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureAtlas.h"

#include <variant>

namespace Cosair {

	class Material {
	public:
		Material(const ShaderRef& shader = s_FlatColorShader) : m_Shader(shader) { }

		static void Init();
		static void NewFrame() { s_SetViewProjMatrix = true; }

		float Bind(RendererData2D& data);

		void RemoveTexture();
		void SetTexture(const Texture2dRef& texture);
		inline const Texture2dRef& GetTexture() const { return m_Texture; }

		// To remove the Sub-Texture, use 'Material::RemoveTexture()'
		void SetSubTexture(const char* subTextureName);
		inline void SetTextureAtlas(const TextureAtlasRef& textureAtlas) { m_TextureAtlas = textureAtlas; }

		inline const TextureCoords& GetTextureCoords() const { return m_TextureCoords; }

		void SetColor(const glm::vec4 color) { m_Color = color; }
		inline const glm::vec4& GetColor() const { return m_Color; }

		inline const ShaderRef& GetShader() const { return m_Shader; }

		template<typename T>
		inline void Set(const std::string& name, const T& value) {
			static_assert(
				std::is_same_v<T, glm::mat4> || std::is_same_v<T, glm::mat3> ||
				std::is_same_v<T, glm::vec4> || std::is_same_v<T, glm::vec3> ||
				std::is_same_v<T, glm::vec2> || std::is_same_v<T, float> ||
				std::is_same_v<T, glm::ivec4> || std::is_same_v<T, glm::ivec3> ||
				std::is_same_v<T, glm::ivec2> || std::is_same_v<T, int> ||
				std::is_same_v<T, int*>,
				"Invalid Type, supported types are: glm::mat4, glm::mat3, glm::vec4, glm::vec3, glm::vec2, float, glm::ivec4, glm::ivec3, glm::ivec2, int and int*"
				);
			m_ShaderVars[name] = value;
		}

	private:
		void SetShaderVars();

	private:
		static ShaderRef s_TextureShader;
		static ShaderRef s_FlatColorShader;

		static bool s_SetViewProjMatrix;

	private:
		ShaderRef m_Shader;

		glm::vec4 m_Color { 1 };
		TextureCoords m_TextureCoords;
		Texture2dRef m_Texture = nullptr;
		TextureAtlasRef m_TextureAtlas = nullptr;

		using ShaderVarValue = std::variant<
			glm::mat4, glm::mat3,
			glm::vec4, glm::vec3, glm::vec2, float,
			glm::ivec4, glm::ivec3, glm::ivec2, int
		>;
		std::unordered_map<std::string, ShaderVarValue> m_ShaderVars;
	};

	using MaterialRef = Ref<Material>;

}