#pragma once

#include "Cosair/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Cosair {

	// VertexBuffer

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const void* data, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		// Sets the data of the vertex buffer using the given parameters at the offset provided (default: 0)
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

		inline virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; };
	private:
		BufferLayout m_Layout;
		uint32_t m_VertexBufferId;
	};

	// IndexBuffer

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		inline virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_Count;
		uint32_t m_IndexBufferId;
	};

	// ShaderStorageBuffer

	class OpenGLShaderStorageBuffer : public ShaderStorageBuffer {
	public:
		OpenGLShaderStorageBuffer(uint32_t size);
		OpenGLShaderStorageBuffer(const void* data, uint32_t size);
		virtual ~OpenGLShaderStorageBuffer() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void BindBase(uint32_t index) override;

		// Sets the data of the shader storage buffer using the given parameters at the offset provided (default: 0)
		virtual void SetData(const uint64_t* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_ShaderStorageBufferId;
	};

}