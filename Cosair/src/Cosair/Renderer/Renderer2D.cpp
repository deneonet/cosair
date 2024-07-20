#include "crpch.h"
#include "Renderer2D.h"

#include "RenderOps.h"
#include "VertexArray.h"
#include "MaterialLibrary.h"

#include <glm/ext/matrix_transform.hpp>

namespace Cosair {

    struct SceneStorage2D {
        glm::mat4 ViewProjectionMatrix;
        VertexArrayRef QuadVertexArray;
    };

    static Scope<SceneStorage2D> s_Data;

    void Renderer2D::Init() {
        s_Data = std::make_unique<SceneStorage2D>();
        s_Data->QuadVertexArray = VertexArray::Create();

        float quadVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Cosair::VertexBufferRef vertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
        vertexBuffer->SetLayout({
            { Cosair::ShaderType::Vec3, "a_Position" },
            { Cosair::ShaderType::Vec2, "a_TexCoord" }
            });

        s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        Cosair::IndexBufferRef indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);
    }

    void Renderer2D::Shutdown() {
        s_Data.reset();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        s_Data->ViewProjectionMatrix = camera.GetViewProjMatrix();
        Material::NewFrame();
    }

    void Renderer2D::EndScene() {}

    void Renderer2D::DrawQuad(const MaterialRef& material) {
        material->Bind(s_Data->ViewProjectionMatrix);
        s_Data->QuadVertexArray->Bind();
        RenderOps::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const char* materialName) {
        const MaterialRef& material = MaterialLibrary::Load(materialName);
        if (material == nullptr) {
            CR_CORE_ERROR("Material '{0}' not loaded", materialName);
            return;
        }

        material->Bind(s_Data->ViewProjectionMatrix);
        s_Data->QuadVertexArray->Bind();
        RenderOps::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const MaterialRef& material, const TransformRef& transform) {
        material->Bind(s_Data->ViewProjectionMatrix, transform);
        s_Data->QuadVertexArray->Bind();
        RenderOps::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const char* materialName, const TransformRef& transform) {
        const MaterialRef& material = MaterialLibrary::Load(materialName);
        if (material == nullptr) {
            CR_CORE_ERROR("Material '{0}' not loaded", materialName);
            return;
        }

        material->Bind(s_Data->ViewProjectionMatrix, transform);
        s_Data->QuadVertexArray->Bind();
        RenderOps::DrawIndexed(s_Data->QuadVertexArray);
    }

}