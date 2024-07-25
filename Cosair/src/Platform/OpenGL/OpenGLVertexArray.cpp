#include "crpch.h"
#include "OpenGLVertexArray.h"

namespace Cosair {

	void OpenGLVertexArray::SetIndexBuffer(const IndexBufferRef& indexBuffer) {
		CR_PROFILE_FUNCTION();

		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddVertexBuffer(const VertexBufferRef& vertexBuffer) {
		CR_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		CR_CORE_ASSERT(layout.GetElements().size(), "Vertex buffer has no layout or the layout is empty");

		for (const auto& element : layout) {
			glEnableVertexAttribArray(m_VertexBufferIndex);
			switch (element.Type) {
			case ShaderType::Int:
			case ShaderType::Int2:
			case ShaderType::Int3:
			case ShaderType::Int4:
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					GetOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)(intptr_t)element.Offset
				);
				m_VertexBufferIndex++;
				break;
			case ShaderType::Matrix3:
			case ShaderType::Matrix4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						GetOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					GetOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(intptr_t)element.Offset
				);
				m_VertexBufferIndex++;
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

}