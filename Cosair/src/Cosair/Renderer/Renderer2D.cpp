#include "crpch.h"
#include "Renderer2D.h"

#include "Libraries.h"
#include "Renderer.h"
#include "RenderOps.h"
#include "VertexArray.h"

namespace Cosair {

	static RendererData2D s_Data;

	void Renderer2D::Init(RendererProps2D props) {
		CR_PROFILE_FUNCTION();
		s_Data = { props.MaxQuads };
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.VerticiesCount * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ Cosair::ShaderType::Float3,  "a_Position" },
			{ Cosair::ShaderType::Float4,  "a_Color" },
			{ Cosair::ShaderType::Float2,  "a_TexCoord" },
			{ Cosair::ShaderType::Float, "a_TexIndex" }
			});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.VerticiesCount];

		Ref<uint32_t> quadIndicies;
		quadIndicies.reset(new uint32_t[s_Data.IndiciesCount]);

		uint32_t offset = 0;
		for (int i = 0; i < s_Data.IndiciesCount; i += 6) {
			quadIndicies.get()[i + 0] = offset + 0;
			quadIndicies.get()[i + 1] = offset + 1;
			quadIndicies.get()[i + 2] = offset + 2;
			quadIndicies.get()[i + 3] = offset + 2;
			quadIndicies.get()[i + 4] = offset + 3;
			quadIndicies.get()[i + 5] = offset + 0;

			offset += 4;
		}

		Cosair::IndexBufferRef quadIndexBuffer = IndexBuffer::Create(quadIndicies.get(), s_Data.IndiciesCount);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

	#ifdef CR_EXT_BINDLESS_TEXS
		s_Data.TextureHandles.reset(new uint64_t[props.MaxBindlessTextures]);
		s_Data.ShaderStorageBuffer = ShaderStorageBuffer::Create(props.MaxBindlessTextures * sizeof(uint64_t));
	#endif

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		delete s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		CR_PROFILE_FUNCTION();

		Material::NewFrame();
		s_Data.ViewProjectionMatrix = camera.GetViewProjMatrix();

		ResetBatch();
	}

	void Renderer2D::EndScene() {
		CR_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::ResetBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	#ifndef CR_EXT_BINDLESS_TEXS
		s_Data.TextureSlotIndex = 0;
	#endif
	}

	void Renderer2D::Flush() {
		CR_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount == 0)
			return;

	#ifdef CR_EXT_BINDLESS_TEXS
		s_Data.ShaderStorageBuffer->BindBase(0);
		if (s_Data.FlushTextureHandles) {
			s_Data.ShaderStorageBuffer->SetData(s_Data.TextureHandles.get(), s_Data.TextureHandlesSize * sizeof(uint64_t));
			s_Data.FlushTextureHandles = false;
		}
	#else
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
	#endif

		RenderOps::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	#ifdef CR_EXT_RENDER_STATS
		s_Data.Stats.DrawCalls++;
	#endif
	}

#ifdef CR_EXT_BINDLESS_TEXS
	void Renderer2D::AddTextureHandle(uint32_t index, uint64_t handleId) {
		CR_PROFILE_FUNCTION();

		s_Data.TextureHandles.get()[index] = handleId;
		s_Data.FlushTextureHandles = true;
		s_Data.TextureHandlesSize++;
	}
#endif

	void Renderer2D::DrawQuad(const MaterialRef& material, const TransformRef& transform) {
		CR_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.IndiciesCount) {
			EndScene();
			ResetBatch();
		}

		glm::vec3 vertexPositions[4];
		if (transform->HasRotation()) {
			const glm::mat4& transformMatrix = transform->GetTransformMatrix();
			vertexPositions[0] = transformMatrix * s_Data.QuadVertexPositions[0];
			vertexPositions[1] = transformMatrix * s_Data.QuadVertexPositions[1];
			vertexPositions[2] = transformMatrix * s_Data.QuadVertexPositions[2];
			vertexPositions[3] = transformMatrix * s_Data.QuadVertexPositions[3];
		} else {
			const glm::vec3& scale = transform->GetScale();
			const glm::vec3& position = transform->GetPosition();
			vertexPositions[0] = { position.x - scale.x * 0.5f, position.y - scale.y * 0.5f, 1 };
			vertexPositions[1] = { position.x + scale.x * 0.5f, position.y - scale.y * 0.5f, 1 };
			vertexPositions[2] = { position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, 1 };
			vertexPositions[3] = { position.x - scale.x * 0.5f, position.y + scale.y * 0.5f, 1 };
		}

		float textureIndex = material->Bind(s_Data);
		TextureCoords texCoords = material->GetTextureCoords();

		s_Data.QuadVertexBufferPtr->Position = vertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = material->GetColor();
		s_Data.QuadVertexBufferPtr->TexCoord = { texCoords.X1, texCoords.Y1 };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = vertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = material->GetColor();
		s_Data.QuadVertexBufferPtr->TexCoord = { texCoords.X2, texCoords.Y2 };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = vertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = material->GetColor();
		s_Data.QuadVertexBufferPtr->TexCoord = { texCoords.X3, texCoords.Y3 };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = vertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = material->GetColor();
		s_Data.QuadVertexBufferPtr->TexCoord = { texCoords.X4, texCoords.Y4 };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	#ifdef CR_EXT_RENDER_STATS
		s_Data.Stats.QuadCount++;
	#endif
	}

	void Renderer2D::DrawQuad(const char* materialName, const TransformRef& transform) {
		CR_PROFILE_FUNCTION();

	#ifndef CR_STRIP_SECURITY_CHECKS
		const MaterialRef& material = MaterialLibrary::Load(materialName);
		if (material == nullptr) {
			CR_CORE_DERROR("Material '{0}' not loaded", materialName);
			return;
		}
		DrawQuad(material, transform);
	#else
		DrawQuad(MaterialLibrary::Load(materialName), transform);
	#endif
	}

#ifdef CR_EXT_RENDER_STATS
	void Renderer2D::ResetStats() { memset(&s_Data.Stats, 0, sizeof(Statistics)); }
	const Statistics& Renderer2D::GetStats() { return s_Data.Stats; }
#else
	void Renderer2D::ResetStats() { }
	const Statistics& Renderer2D::GetStats() { return s_Data.Stats; }
#endif

}