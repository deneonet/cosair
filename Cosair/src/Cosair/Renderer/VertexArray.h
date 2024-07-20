#pragma once

#include "Buffer.h"

namespace Cosair {

	class VertexArray {
	public:
		inline virtual ~VertexArray() = default;;

		inline virtual void Bind() const = 0;
		inline virtual void Unbind() const = 0;

		virtual void SetIndexBuffer(const IndexBufferRef& indexBuffer) = 0;
		virtual void AddVertexBuffer(const VertexBufferRef& vertexBuffer) = 0;

		inline virtual const IndexBufferRef& GetIndexBuffer() = 0;
		inline virtual const std::vector<VertexBufferRef>& GetVertexBuffers() = 0;

		static Ref<VertexArray> Create();
	};

	using VertexArrayRef = Ref<VertexArray>;
}