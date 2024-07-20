#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include <variant>
#include <unordered_map>

namespace Cosair {

    class Material {
    public:
        Material(const ShaderRef& shader, const TransformRef& transform = std::make_shared<Transform>())
            : m_Shader(shader), m_Transform(transform) {}
        Material(const TransformRef& transform = std::make_shared<Transform>())
            : Material(s_FlatColorShader, transform) {}

        static void Init();
        static void NewFrame() { s_SetViewProjMatrix = true; }

        void Bind(const glm::mat4& viewProjectionMatrix, const TransformRef& transform);
        void Bind(const glm::mat4& viewProjectionMatrix) { Bind(viewProjectionMatrix, m_Transform); }

        void RemoveTexture();
        void SetTexture(Texture2dRef texture);

        void SetColor(const glm::vec4 color) { m_Color = color; }
        void SetTransform(const TransformRef& transform) { m_Transform = transform; }

        const glm::vec4& GetColor() const { return m_Color; }
        const ShaderRef& GetShader() const { return m_Shader; }
        const Texture2dRef& GetTexture() const { return m_Texture; }
        const TransformRef& GetTransform() const { return m_Transform; }

        template<typename T>
        void Set(const std::string& name, const T& value) {
            static_assert(
                std::is_same_v<T, glm::mat4> || std::is_same_v<T, glm::mat3> ||
                std::is_same_v<T, glm::vec4> || std::is_same_v<T, glm::vec3> ||
                std::is_same_v<T, glm::vec2> || std::is_same_v<T, float> ||
                std::is_same_v<T, glm::ivec4> || std::is_same_v<T, glm::ivec3> ||
                std::is_same_v<T, glm::ivec2> || std::is_same_v<T, int>,
                "Invalid Type, supported types are: glm::mat4, glm::mat3, glm::vec4, glm::vec3, glm::vec2, float, glm::ivec4, glm::ivec3, glm::ivec2, and int"
                );
            m_Uniforms[name] = value;
        }

        void SetScale(const glm::vec3& scale) { m_Transform->SetScale(scale); }
        void SetPosition(const glm::vec3& position) { m_Transform->SetPosition(position); }
        void SetRotation(const glm::vec3& rotation) { m_Transform->SetRotation(rotation); }

        const glm::vec3& GetScale() const { return m_Transform->GetScale(); }
        const glm::vec3& GetPosition() const { return m_Transform->GetPosition(); }
        const glm::vec3& GetRotation() const { return m_Transform->GetRotation(); }

        void SetScaleX(float x) { m_Transform->SetScaleX(x); }
        void SetScaleY(float y) { m_Transform->SetScaleY(y); }
        void SetScaleZ(float z) { m_Transform->SetScaleZ(z); }
        void SetPositionX(float x) { m_Transform->SetPositionX(x); }
        void SetPositionY(float y) { m_Transform->SetPositionY(y); }
        void SetPositionZ(float z) { m_Transform->SetPositionZ(z); }
        void SetRotationX(float x) { m_Transform->SetRotationX(x); }
        void SetRotationY(float y) { m_Transform->SetRotationY(y); }
        void SetRotationZ(float z) { m_Transform->SetRotationZ(z); }

        float GetScaleX() const { return m_Transform->GetScaleX(); }
        float GetScaleY() const { return m_Transform->GetScaleY(); }
        float GetScaleZ() const { return m_Transform->GetScaleZ(); }
        float GetPositionX() const { return m_Transform->GetPositionX(); }
        float GetPositionY() const { return m_Transform->GetPositionY(); }
        float GetPositionZ() const { return m_Transform->GetPositionZ(); }
        float GetRotationX() const { return m_Transform->GetRotationX(); }
        float GetRotationY() const { return m_Transform->GetRotationY(); }
        float GetRotationZ() const { return m_Transform->GetRotationZ(); }

        const glm::mat4& GetTransformMatrix() const { return m_Transform->GetTransformMatrix(); }

    private:
        void UploadUniforms();

    private:
        static ShaderRef s_TextureShader;
        static ShaderRef s_FlatColorShader;

        static glm::vec4 s_LastColor;
        static uint32_t s_LastShaderId;

        static bool s_SetViewProjMatrix;

    private:
        ShaderRef m_Shader;
        TransformRef m_Transform;

        glm::vec4 m_Color{ 1 };
        Texture2dRef m_Texture = nullptr;

        using UniformValue = std::variant<
            glm::mat4, glm::mat3, glm::vec4, glm::vec3, glm::vec2, float,
            glm::ivec4, glm::ivec3, glm::ivec2, int
        >;
        std::unordered_map<std::string, UniformValue> m_Uniforms;
    };

    using MaterialRef = Ref<Material>;

}