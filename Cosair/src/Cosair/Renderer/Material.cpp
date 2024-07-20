#include "crpch.h"
#include "Material.h"
#include "Transform.h"

namespace Cosair {

    ShaderRef Material::s_TextureShader;
    ShaderRef Material::s_FlatColorShader;

    glm::vec4 Material::s_LastColor;
    uint32_t Material::s_LastShaderId;

    bool Material::s_SetViewProjMatrix = true;

    void Material::Init() {
        s_TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
    }

    void Material::Bind(const glm::mat4& viewProjectionMatrix, const TransformRef& transform) {
        if (m_Texture)
            m_Texture->Bind();

        uint32_t shaderId = m_Shader->GetId();
        if (s_LastShaderId != shaderId) {
            m_Shader->Bind();
            s_LastShaderId = shaderId;
        }

        if (s_SetViewProjMatrix) {
            m_Shader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
            s_SetViewProjMatrix = false;
        }

        if (s_LastColor != m_Color) {
            m_Shader->UploadUniformVec4("u_Color", m_Color);
            s_LastColor = m_Color;
        }

        m_Shader->UploadUniformMat4("u_Transform", transform->GetTransformMatrix());
        UploadUniforms();
    }

    void Material::RemoveTexture() {
        m_Texture = nullptr;
        if (m_Shader->GetId() == s_TextureShader->GetId())
            m_Shader = s_FlatColorShader;
    }

    void Material::SetTexture(Texture2dRef texture) {
        m_Texture = texture;
        if (m_Shader->GetId() == s_FlatColorShader->GetId())
            m_Shader = s_TextureShader;
    }

    void Material::UploadUniforms() {
        for (const auto& [name, value] : m_Uniforms) {
            std::visit([&](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, glm::mat4>)
                    m_Shader->UploadUniformMat4(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::mat3>)
                    m_Shader->UploadUniformMat3(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::vec4>)
                    m_Shader->UploadUniformVec4(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    m_Shader->UploadUniformVec3(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    m_Shader->UploadUniformVec2(name.c_str(), val);
                else if constexpr (std::is_same_v<T, float>)
                    m_Shader->UploadUniformFloat(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::ivec4>)
                    m_Shader->UploadUniformInt4(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::ivec3>)
                    m_Shader->UploadUniformInt3(name.c_str(), val);
                else if constexpr (std::is_same_v<T, glm::ivec2>)
                    m_Shader->UploadUniformInt2(name.c_str(), val);
                else if constexpr (std::is_same_v<T, int>)
                    m_Shader->UploadUniformInt(name.c_str(), val);
                }, value);
        }
    }

}