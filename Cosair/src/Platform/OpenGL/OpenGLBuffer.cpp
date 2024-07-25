#include "crpch.h"
#include "OpenGLBuffer.h"

namespace Cosair {

	////////////////////////////////////
	// VertexBuffer ////////////////////
	////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		CR_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size) {
		CR_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		CR_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_VertexBufferId);
	}

	void OpenGLVertexBuffer::Bind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	}

	void OpenGLVertexBuffer::Unbind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(0, m_VertexBufferId);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		CR_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	////////////////////////////////////
	// IndexBuffer /////////////////////
	////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t count) : m_Count(count) {
		CR_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_IndexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_IndexBufferId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		CR_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_IndexBufferId);
	}

	void OpenGLIndexBuffer::Bind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
	}

	void OpenGLIndexBuffer::Unbind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(0, m_IndexBufferId);
	}

	////////////////////////////////////
	// ShaderStorageBuffer /////////////
	////////////////////////////////////

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size) {
		CR_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_ShaderStorageBufferId);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ShaderStorageBufferId);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const void* data, uint32_t size) {
		CR_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_ShaderStorageBufferId);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ShaderStorageBufferId);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer() {
		CR_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_ShaderStorageBufferId);
	}

	void OpenGLShaderStorageBuffer::Bind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ShaderStorageBufferId);
	}

	void OpenGLShaderStorageBuffer::BindBase(uint32_t index) {
		CR_PROFILE_FUNCTION();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ShaderStorageBufferId);
	}

	void OpenGLShaderStorageBuffer::Unbind() {
		CR_PROFILE_FUNCTION();
		glBindBuffer(0, m_ShaderStorageBufferId);
	}

	void OpenGLShaderStorageBuffer::SetData(const uint64_t* data, uint32_t size, uint32_t offset) {
		CR_PROFILE_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ShaderStorageBufferId);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	}

}