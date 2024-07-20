#include "crpch.h"
#include "OpenGLVertexArray.h"

namespace Cosair {

	void OpenGLVertexArray::SetIndexBuffer(const IndexBufferRef& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddVertexBuffer(const VertexBufferRef& vertexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		CR_CORE_ASSERT(layout.GetElements().size(), "Vertex buffer has no layout or the layout is empty");

		for (const auto& element : layout) {
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				GetOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset
			);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

}