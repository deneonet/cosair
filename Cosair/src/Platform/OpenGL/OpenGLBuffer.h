#pragma once

#include "Cosair/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Cosair {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const float* data, uint32_t size);
		inline virtual ~OpenGLVertexBuffer() override { glDeleteBuffers(1, &m_VertexBufferID); }

		inline virtual void Bind() const override { glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID); }
		inline virtual void Unbind() const override { glBindBuffer(0, m_VertexBufferID); }

		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		BufferLayout m_Layout;
		uint32_t m_VertexBufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const uint32_t* data, uint32_t count);
		inline virtual ~OpenGLIndexBuffer() override { glDeleteBuffers(1, &m_IndexBufferID); }

		inline virtual uint32_t GetCount() const { return m_Count; }

		inline virtual void Bind() const override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID); }
		inline virtual void Unbind() const override { glBindBuffer(0, m_IndexBufferID); }
	private:
		uint32_t m_IndexBufferID;
		uint32_t m_Count;
	};

}