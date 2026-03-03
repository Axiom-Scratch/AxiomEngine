#include "Renderer/RHI/OpenGL/OpenGLBuffer.h"

#include "Renderer/RHI/RHI.h"

#include <glad/glad.h>

#include <memory>

namespace Axiom
{
    std::unique_ptr<RHIVertexBuffer> RHIVertexBuffer::Create(const float* vertices, uint32_t size)
    {
        switch (RHI::GetAPIType())
        {
            case RendererAPIType::OpenGL:
                return std::make_unique<OpenGLVertexBuffer>(vertices, size);
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }

    std::unique_ptr<RHIIndexBuffer> RHIIndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (RHI::GetAPIType())
        {
            case RendererAPIType::OpenGL:
                return std::make_unique<OpenGLIndexBuffer>(indices, count);
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::GetCount() const
    {
        return m_Count;
    }
}
