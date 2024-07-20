#pragma once

#include "OpenGLShader.h"
#include "Cosair/Renderer/VertexArray.h"

namespace Cosair {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray() { glCreateVertexArrays(1, &m_VertexArrayID); }
		inline virtual ~OpenGLVertexArray() override { glDeleteVertexArrays(1, &m_VertexArrayID); }

		inline virtual void Bind() const override { glBindVertexArray(m_VertexArrayID); }
		inline virtual void Unbind() const override { glBindVertexArray(0); }

		virtual void SetIndexBuffer(const IndexBufferRef& indexBuffer) override;
		virtual void AddVertexBuffer(const VertexBufferRef& vertexBuffer) override;

		inline virtual const IndexBufferRef& GetIndexBuffer() override { return m_IndexBuffer; }
		inline virtual const std::vector<VertexBufferRef>& GetVertexBuffers() override { return m_VertexBuffers; }
	private:
		uint32_t m_VertexArrayID;
		uint32_t m_VertexBufferIndex = 0;

		IndexBufferRef m_IndexBuffer;
		std::vector<VertexBufferRef> m_VertexBuffers;
	};

}