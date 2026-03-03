#include "RHI/OpenGL/OpenGLVertexArray.h"

#include "RHI/RHIBuffer.h"

#include <glad/glad.h>
#include <cstdint>

namespace Axiom
{
    namespace
    {
        GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                    return GL_FLOAT;
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                    return GL_INT;
                case ShaderDataType::Bool:
                    return GL_BOOL;
                case ShaderDataType::None:
                    return GL_FLOAT;
            }
            return GL_FLOAT;
        }

        bool IsIntegerType(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                    return true;
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                case ShaderDataType::None:
                    return false;
            }
            return false;
        }

        uint32_t GetMatrixColumnCount(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Mat3: return 3;
                case ShaderDataType::Mat4: return 4;
                default: return 0;
            }
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const RHIVertexBuffer& vertexBuffer, const BufferLayout& layout)
    {
        Bind();
        vertexBuffer.Bind();

        for (const auto& element : layout)
        {
            const GLenum glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
            const uint32_t columnCount = GetMatrixColumnCount(element.Type);
            if (columnCount > 0)
            {
                for (uint32_t column = 0; column < columnCount; ++column)
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    const std::uintptr_t offset = static_cast<std::uintptr_t>(element.Offset + sizeof(float) * columnCount * column);
                    glVertexAttribPointer(m_VertexBufferIndex,
                        static_cast<GLint>(element.GetComponentCount()),
                        glBaseType,
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        static_cast<GLsizei>(layout.GetStride()),
                        reinterpret_cast<const void*>(offset));
                    ++m_VertexBufferIndex;
                }
                continue;
            }

            glEnableVertexAttribArray(m_VertexBufferIndex);
            if (IsIntegerType(element.Type))
            {
                glVertexAttribIPointer(m_VertexBufferIndex,
                    static_cast<GLint>(element.GetComponentCount()),
                    glBaseType,
                    static_cast<GLsizei>(layout.GetStride()),
                    reinterpret_cast<const void*>(static_cast<std::uintptr_t>(element.Offset)));
            }
            else
            {
                glVertexAttribPointer(m_VertexBufferIndex,
                    static_cast<GLint>(element.GetComponentCount()),
                    glBaseType,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    static_cast<GLsizei>(layout.GetStride()),
                    reinterpret_cast<const void*>(static_cast<std::uintptr_t>(element.Offset)));
            }
            ++m_VertexBufferIndex;
        }
    }

    void OpenGLVertexArray::SetIndexBuffer(const RHIIndexBuffer& indexBuffer)
    {
        Bind();
        indexBuffer.Bind();
    }
}
