#include "crpch.h"
#include "Material.h"

#include "Renderer.h"
#include "Renderer2D.h"

namespace Cosair {

	ShaderRef Material::s_TextureShader;
	ShaderRef Material::s_FlatColorShader;

	bool Material::s_SetViewProjMatrix = true;

	void Material::Init() {
		CR_PROFILE_FUNCTION();

		s_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	#ifdef CR_EXT_BINDLESS_TEXS
		s_TextureShader = Shader::Create("assets/shaders/Bindless.glsl");
	#else
		s_TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	#endif
	}

	float Material::Bind(RendererData2D& data) {
		CR_PROFILE_FUNCTION();

		static uint32_t s_LastShaderId;
		uint32_t shaderId = m_Shader->GetProgramId();
		if (s_LastShaderId != shaderId) {
			m_Shader->Bind();

		#ifndef CR_EXT_BINDLESS_TEXS
			int32_t samplers[32];
			for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
				samplers[i] = i;
			m_Shader->SetIntArray("u_Textures", samplers, data.MaxTextureSlots);
		#endif

			s_LastShaderId = shaderId;
			s_SetViewProjMatrix = true;
		}

		if (s_SetViewProjMatrix) {
			m_Shader->SetMatrix4("u_ViewProjection", data.ViewProjectionMatrix);
			s_SetViewProjMatrix = false;
		}

		if (!m_ShaderVars.empty())
			SetShaderVars();

	#ifdef CR_EXT_BINDLESS_TEXS
		return m_Texture->GetIndex();
	#else
		float textureIndex = 0;

		if (m_Texture) {
			if (data.TextureSlotIndex >= data.MaxTextureSlots) {
				Renderer2D::EndScene();
				Renderer2D::ResetBatch();
			}

			for (uint32_t i = 0; i < data.TextureSlotIndex; i++) {
				if (data.TextureSlots[i]->GetTextureId() == m_Texture->GetTextureId()) {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0) {
				uint32_t freeTextureSlot = data.TextureSlotIndex;
				textureIndex = (float)freeTextureSlot;
				data.TextureSlots[freeTextureSlot] = m_Texture;
				data.TextureSlotIndex++;
			}
		}
		return textureIndex;
	#endif
	}

	void Material::RemoveTexture() {
		CR_PROFILE_FUNCTION();

		m_Texture = nullptr;
		if (m_Shader->GetProgramId() == s_TextureShader->GetProgramId())
			m_Shader = s_FlatColorShader;
	}

	void Material::SetTexture(const Texture2dRef& texture) {
		CR_PROFILE_FUNCTION();

		m_Texture = texture;
		m_TextureCoords = TextureCoords { 0, 0, 1, 0, 1, 1, 0, 1 };
		if (m_Shader->GetProgramId() == s_FlatColorShader->GetProgramId())
			m_Shader = s_TextureShader;
	}

	void Material::SetSubTexture(const char* subTextureName) {
		CR_PROFILE_FUNCTION();
	#ifndef CR_STRIP_SECURITY_CHECKS
		if (!m_TextureAtlas.get()) {
			CR_CORE_DERROR("Trying to use 'Material::SetSubTexture', but no TextureAtlas is set");
			return;
		}
	#endif

		m_Texture = m_TextureAtlas->GetTexture();
		if (m_Shader->GetProgramId() == s_FlatColorShader->GetProgramId())
			m_Shader = s_TextureShader;
		// TODO: Check if a sub texture with that name even exists
		m_TextureCoords = m_TextureAtlas->LoadSubTexture(subTextureName);
	}

	void Material::SetShaderVars() {
		CR_PROFILE_FUNCTION();

		for (const auto& [name, value] : m_ShaderVars) {
			std::visit([&](auto&& val) {
				using T = std::decay_t<decltype(val)>;
				if constexpr (std::is_same_v<T, glm::mat4>)
					m_Shader->SetMatrix4(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::mat3>)
					m_Shader->SetMatrix3(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::vec4>)
					m_Shader->SetFloat4(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::vec3>)
					m_Shader->SetFloat3(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::vec2>)
					m_Shader->SetFloat2(name.c_str(), val);
				else if constexpr (std::is_same_v<T, float>)
					m_Shader->SetFloat(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::ivec4>)
					m_Shader->SetInt4(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::ivec3>)
					m_Shader->SetInt3(name.c_str(), val);
				else if constexpr (std::is_same_v<T, glm::ivec2>)
					m_Shader->SetInt2(name.c_str(), val);
				else if constexpr (std::is_same_v<T, int>)
					m_Shader->SetInt(name.c_str(), val);
				else if constexpr (std::is_same_v<T, int*>)
					m_Shader->SetIntArray(name.c_str(), val, sizeof(val) / sizeof(int));
			}, value);
		}
	}

}