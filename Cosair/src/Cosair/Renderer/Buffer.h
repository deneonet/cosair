#pragma once

#include <initializer_list>

#include "shader.h"

namespace cosair {

struct BufferElement {
  size_t size;
  size_t offset;
  ShaderType type;
  bool normalized;
  std::string name;

  BufferElement(ShaderType type, const std::string& name,
                bool normalized = false)
      : name(name),
        type(type),
        offset(0),
        normalized(normalized),
        size(GetShaderTypeSize(type)) {}

  uint8_t GetComponentCount() const {
    switch (type) {
      case ShaderType::kFloat:
        return 1;
      case ShaderType::kFloat2:
        return 2;
      case ShaderType::kFloat3:
        return 3;
      case ShaderType::kFloat4:
        return 4;
      case ShaderType::kMatrix3:
        return 3;
      case ShaderType::kMatrix4:
        return 4;
      case ShaderType::kInt:
        return 1;
      case ShaderType::kInt2:
        return 2;
      case ShaderType::kInt3:
        return 3;
      case ShaderType::kInt4:
        return 4;
      case ShaderType::kBool:
        return 1;
      default:
        return 0;
    }
  }
};

class BufferLayout {
 public:
  BufferLayout() = default;
  BufferLayout(const std::initializer_list<BufferElement>& elements)
      : elements_(elements) {
    size_t offset = 0;
    for (BufferElement& element : elements_) {
      element.offset = offset;
      offset += element.size;
      stride_ += element.size;
    }
  }

  inline std::vector<BufferElement>::iterator begin() {
    return elements_.begin();
  }
  inline std::vector<BufferElement>::iterator end() { return elements_.end(); }

  inline std::vector<BufferElement>::reverse_iterator rbegin() {
    return elements_.rbegin();
  }
  inline std::vector<BufferElement>::reverse_iterator rend() {
    return elements_.rend();
  }

  inline std::vector<BufferElement>::const_iterator begin() const {
    return elements_.begin();
  }
  inline std::vector<BufferElement>::const_iterator end() const {
    return elements_.end();
  }

  inline std::vector<BufferElement>::const_reverse_iterator rbegin() const {
    return elements_.rbegin();
  }
  inline std::vector<BufferElement>::const_reverse_iterator rend() const {
    return elements_.rend();
  }

  inline size_t GetStride() const { return stride_; }
  inline const std::vector<BufferElement>& GetElements() const {
    return elements_;
  }

 private:
  size_t stride_ = 0;
  std::vector<BufferElement> elements_;
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void SetData(const void* data, size_t size, size_t offset = 0) = 0;

  inline virtual uint32_t GetId() const = 0;

  inline virtual const BufferLayout& GetLayout() const = 0;
  inline virtual void SetLayout(const BufferLayout& layout) = 0;

  static Ref<VertexBuffer> Create(size_t size);
  static Ref<VertexBuffer> Create(const void* data, size_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  inline virtual uint32_t GetId() const = 0;
  inline virtual size_t GetCount() const = 0;

  static Ref<IndexBuffer> Create(const void* data, size_t count);
};

// ShdSBuffer = Shader Storage Buffer
class ShdSBuffer {
 public:
  virtual ~ShdSBuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void BindBase(uint32_t index) = 0;

  virtual void SetData(const void* data, size_t size, size_t offset = 0) = 0;

  inline virtual uint32_t GetId() const = 0;

  static Ref<ShdSBuffer> Create(size_t size);
  static Ref<ShdSBuffer> Create(const void* data, size_t size);
};

using ShdSBufferRef = Ref<ShdSBuffer>;
using IndexBufferRef = Ref<IndexBuffer>;
using VertexBufferRef = Ref<VertexBuffer>;

}  // namespace cosair