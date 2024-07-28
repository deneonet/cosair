#include "crpch.h"

#include "opengl_vertex_array.h"

namespace cosair {

OpenGLVertexArray::OpenGLVertexArray() {
  CR_PROFILE_FUNCTION();
  glCreateVertexArrays(1, &opengl_id_);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  CR_PROFILE_FUNCTION();
  glDeleteVertexArrays(1, &opengl_id_);
}

void OpenGLVertexArray::Bind() {
  CR_PROFILE_FUNCTION();
  glBindVertexArray(opengl_id_);
}

void OpenGLVertexArray::Unbind() {
  CR_PROFILE_FUNCTION();
  glBindVertexArray(0);
}

void OpenGLVertexArray::SetIndexBuffer(const IndexBufferRef& index_buffer) {
  CR_PROFILE_FUNCTION();
  glBindVertexArray(opengl_id_);
  index_buffer->Bind();
  index_buffer_ = index_buffer;
}

void OpenGLVertexArray::AddVertexBuffer(const VertexBufferRef& vertex_buffer) {
  CR_PROFILE_FUNCTION();
  glBindVertexArray(opengl_id_);
  vertex_buffer->Bind();

  const BufferLayout& layout = vertex_buffer->GetLayout();
  CR_CORE_ASSERT(layout.GetElements().size(), "VertexBuffer has no layout");

  for (const BufferElement& element : layout) {
    glEnableVertexAttribArray(vertex_buffer_index_);
    switch (element.type) {
      case ShaderType::kInt:
      case ShaderType::kInt2:
      case ShaderType::kInt3:
      case ShaderType::kInt4:
        glVertexAttribIPointer(
            vertex_buffer_index_, element.GetComponentCount(),
            GetOpenGLBaseType(element.type), (GLsizei)layout.GetStride(),
            (const void*)(intptr_t)element.offset);
        vertex_buffer_index_++;
        break;
      case ShaderType::kMatrix3:
      case ShaderType::kMatrix4: {
        uint8_t count = element.GetComponentCount();
        for (uint8_t i = 0; i < count; i++) {
          glEnableVertexAttribArray(vertex_buffer_index_);
          glVertexAttribPointer(vertex_buffer_index_, count,
                                GetOpenGLBaseType(element.type),
                                element.normalized ? GL_TRUE : GL_FALSE,
                                (GLsizei)layout.GetStride(),
                                (const void*)(sizeof(float) * count * i));
          glVertexAttribDivisor(vertex_buffer_index_, 1);
          vertex_buffer_index_++;
        }
        break;
      }
      default:
        glVertexAttribPointer(vertex_buffer_index_, element.GetComponentCount(),
                              GetOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              (GLsizei)layout.GetStride(),
                              (const void*)(intptr_t)element.offset);
        vertex_buffer_index_++;
    }
  }

  vertex_buffers_.push_back(vertex_buffer);
}

}  // namespace cosair