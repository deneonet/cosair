#pragma once

#include "Shader.h"
#include <string>
#include <vector>
#include <initializer_list>

namespace Cosair {

    struct BufferElement {
        std::string Name;
        uint32_t Size;
        uint32_t Offset;
        ShaderType Type;
        bool Normalized;

        BufferElement(ShaderType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(GetShaderTypeSize(type)), Offset(0), Normalized(normalized) {}

        uint32_t GetComponentCount() const {
            switch (Type) {
            case ShaderType::Float:    return 1;
            case ShaderType::Vec2:     return 2;
            case ShaderType::Vec3:     return 3;
            case ShaderType::Vec4:     return 4;
            case ShaderType::Mat3:     return 3 * 3;
            case ShaderType::Mat4:     return 4 * 4;
            case ShaderType::Int:      return 1;
            case ShaderType::IVec2:    return 2;
            case ShaderType::IVec3:    return 3;
            case ShaderType::IVec4:    return 4;
            case ShaderType::Bool:     return 1;
            default:                   return 0;
            }
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements) {
            uint32_t offset = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

        inline uint32_t GetStride() const { return m_Stride; }
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
    private:
        uint32_t m_Stride = 0;
        std::vector<BufferElement> m_Elements;
    };

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> Create(const float* data, uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(const uint32_t* data, uint32_t count);
    };

    using IndexBufferRef = Ref<IndexBuffer>;
    using VertexBufferRef = Ref<VertexBuffer>;

}