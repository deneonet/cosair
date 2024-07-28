#include "crpch.h"

#include "opengl_buffer.h"

namespace cosair {

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) {
  CR_PROFILE_FUNCTION();
  glCreateBuffers(1, &opengl_id_);
  glBindBuffer(GL_ARRAY_BUFFER, opengl_id_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, size_t size) {
  CR_PROFILE_FUNCTION();
  glCreateBuffers(1, &opengl_id_);
  glBindBuffer(GL_ARRAY_BUFFER, opengl_id_);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  CR_PROFILE_FUNCTION();
  glDeleteBuffers(1, &opengl_id_);
}

void OpenGLVertexBuffer::Bind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(GL_ARRAY_BUFFER, opengl_id_);
}

void OpenGLVertexBuffer::Unbind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(0, opengl_id_);
}

void OpenGLVertexBuffer::SetData(const void* data, size_t size, size_t offset) {
  CR_PROFILE_FUNCTION();
  glBindBuffer(GL_ARRAY_BUFFER, opengl_id_);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const void* data, size_t count)
    : count_(count) {
  CR_PROFILE_FUNCTION();
  glCreateBuffers(1, &opengl_id_);
  glBindBuffer(GL_ARRAY_BUFFER, opengl_id_);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  CR_PROFILE_FUNCTION();
  glDeleteBuffers(1, &opengl_id_);
}

void OpenGLIndexBuffer::Bind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_id_);
}

void OpenGLIndexBuffer::Unbind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(0, opengl_id_);
}

OpenGLShdSBuffer::OpenGLShdSBuffer(size_t size) {
  CR_PROFILE_FUNCTION();
  glCreateBuffers(1, &opengl_id_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, opengl_id_);
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLShdSBuffer::OpenGLShdSBuffer(const void* data, size_t size) {
  CR_PROFILE_FUNCTION();
  glCreateBuffers(1, &opengl_id_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, opengl_id_);
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLShdSBuffer::~OpenGLShdSBuffer() {
  CR_PROFILE_FUNCTION();
  glDeleteBuffers(1, &opengl_id_);
}

void OpenGLShdSBuffer::Bind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, opengl_id_);
}

void OpenGLShdSBuffer::BindBase(uint32_t index) {
  CR_PROFILE_FUNCTION();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, opengl_id_);
}

void OpenGLShdSBuffer::Unbind() {
  CR_PROFILE_FUNCTION();
  glBindBuffer(0, opengl_id_);
}

void OpenGLShdSBuffer::SetData(const void* data, size_t size, size_t offset) {
  CR_PROFILE_FUNCTION();
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, opengl_id_);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
}

}  // namespace cosair