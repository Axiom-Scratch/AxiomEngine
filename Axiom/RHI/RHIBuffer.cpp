#include "RHI/RHIBuffer.h"

#include "RHI/OpenGL/OpenGLBuffer.h"

namespace Axiom
{
    uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::None: return 0;
        }
        return 0;
    }

    BufferElement::BufferElement(ShaderDataType type, std::string name, bool normalized)
        : Name(std::move(name))
        , Type(type)
        , Size(ShaderDataTypeSize(type))
        , Normalized(normalized)
    {
    }

    uint32_t BufferElement::GetComponentCount() const
    {
        switch (Type)
        {
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3;
            case ShaderDataType::Mat4: return 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::None: return 0;
        }
        return 0;
    }

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
        : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    uint32_t BufferLayout::GetStride() const
    {
        return m_Stride;
    }

    const std::vector<BufferElement>& BufferLayout::GetElements() const
    {
        return m_Elements;
    }

    std::vector<BufferElement>::iterator BufferLayout::begin()
    {
        return m_Elements.begin();
    }

    std::vector<BufferElement>::iterator BufferLayout::end()
    {
        return m_Elements.end();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::begin() const
    {
        return m_Elements.begin();
    }

    std::vector<BufferElement>::const_iterator BufferLayout::end() const
    {
        return m_Elements.end();
    }

    void BufferLayout::CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(const float* vertices, uint32_t size)
    {
        return std::make_unique<OpenGLVertexBuffer>(vertices, size);
    }

    std::unique_ptr<RHIIndexBuffer> RHIIndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        return std::make_unique<OpenGLIndexBuffer>(indices, count);
    }
}
