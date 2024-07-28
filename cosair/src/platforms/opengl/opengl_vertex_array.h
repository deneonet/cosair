#pragma once

#include "cosair/renderer/vertex_array.h"
#include "opengl_shader.h"

namespace cosair {

class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray() override;

  virtual void Bind() override;
  virtual void Unbind() override;

  virtual void SetIndexBuffer(const IndexBufferRef& index_buffer) override;
  virtual void AddVertexBuffer(const VertexBufferRef& vertex_buffer) override;

  inline virtual const IndexBufferRef& GetIndexBuffer() override {
    return index_buffer_;
  }
  inline virtual const std::vector<VertexBufferRef>& GetVertexBuffers()
      override {
    return vertex_buffers_;
  }

 private:
  uint32_t opengl_id_;  // The OpenGL Identifier of this VertexArray
  uint32_t vertex_buffer_index_ = 0;

  IndexBufferRef index_buffer_;
  std::vector<VertexBufferRef> vertex_buffers_;
};

}  // namespace cosair