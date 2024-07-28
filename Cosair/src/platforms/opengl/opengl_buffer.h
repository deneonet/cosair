#pragma once

#include <glad/glad.h>

#include "cosair/renderer/buffer.h"

namespace cosair {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(size_t size);
  OpenGLVertexBuffer(const void* data, size_t size);
  virtual ~OpenGLVertexBuffer() override;

  virtual void Bind() override;
  virtual void Unbind() override;

  virtual void SetData(const void* data, size_t size,
                       size_t offset = 0) override;

  inline virtual uint32_t GetId() const override { return opengl_id_; }

  inline virtual void SetLayout(const BufferLayout& layout) override {
    layout_ = layout;
  }
  inline virtual const BufferLayout& GetLayout() const override {
    return layout_;
  };

 private:
  BufferLayout layout_;
  uint32_t opengl_id_;  // The OpenGL Identifier of this VertexBuffer
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(const void* data, size_t count);
  virtual ~OpenGLIndexBuffer() override;

  virtual void Bind() override;
  virtual void Unbind() override;

  inline virtual size_t GetCount() const override { return count_; }
  inline virtual uint32_t GetId() const override { return opengl_id_; }

 private:
  size_t count_;
  uint32_t opengl_id_;  // The OpenGL Identifier of this IndexBuffer
};

class OpenGLShdSBuffer : public ShdSBuffer {
 public:
  OpenGLShdSBuffer(size_t size);
  OpenGLShdSBuffer(const void* data, size_t size);
  virtual ~OpenGLShdSBuffer() override;

  virtual void Bind() override;
  virtual void Unbind() override;
  virtual void BindBase(uint32_t index) override;

  virtual void SetData(const void* data, size_t size,
                       size_t offset = 0) override;
  inline virtual uint32_t GetId() const override { return opengl_id_; }

 private:
  uint32_t opengl_id_;  // The OpenGL Identifier of this ShaderStorageBuffer
                        // (ShdSBuffer)
};

}  // namespace cosair