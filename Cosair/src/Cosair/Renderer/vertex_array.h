#pragma once

#include "buffer.h"

namespace cosair {

class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetIndexBuffer(const IndexBufferRef& index_buffer) = 0;
  virtual void AddVertexBuffer(const VertexBufferRef& vertex_buffer) = 0;

  inline virtual const IndexBufferRef& GetIndexBuffer() = 0;
  inline virtual const std::vector<VertexBufferRef>& GetVertexBuffers() = 0;

  static Ref<VertexArray> Create();
};

using VertexArrayRef = Ref<VertexArray>;
}  // namespace cosair