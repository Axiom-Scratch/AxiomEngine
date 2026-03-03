#include "Renderer/Buffer.h"

namespace Axiom
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size)
    {
        return std::shared_ptr<VertexBuffer>(new VertexBuffer(RHIVertexBuffer::Create(vertices, size)));
    }

    VertexBuffer::VertexBuffer(std::unique_ptr<RHIVertexBuffer> buffer)
        : m_Buffer(std::move(buffer))
    {
    }

    void VertexBuffer::Bind() const
    {
        m_Buffer->Bind();
    }

    void VertexBuffer::Unbind() const
    {
        m_Buffer->Unbind();
    }

    void VertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    const BufferLayout& VertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    RHIVertexBuffer& VertexBuffer::GetRHI()
    {
        return *m_Buffer;
    }

    const RHIVertexBuffer& VertexBuffer::GetRHI() const
    {
        return *m_Buffer;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        return std::shared_ptr<IndexBuffer>(new IndexBuffer(RHIIndexBuffer::Create(indices, count)));
    }

    IndexBuffer::IndexBuffer(std::unique_ptr<RHIIndexBuffer> buffer)
        : m_Buffer(std::move(buffer))
    {
    }

    void IndexBuffer::Bind() const
    {
        m_Buffer->Bind();
    }

    void IndexBuffer::Unbind() const
    {
        m_Buffer->Unbind();
    }

    uint32_t IndexBuffer::GetCount() const
    {
        return m_Buffer->GetCount();
    }

    RHIIndexBuffer& IndexBuffer::GetRHI()
    {
        return *m_Buffer;
    }

    const RHIIndexBuffer& IndexBuffer::GetRHI() const
    {
        return *m_Buffer;
    }

    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::shared_ptr<VertexArray>(new VertexArray(RHIVertexArray::Create()));
    }

    VertexArray::VertexArray(std::unique_ptr<RHIVertexArray> vertexArray)
        : m_VertexArray(std::move(vertexArray))
    {
    }

    void VertexArray::Bind() const
    {
        m_VertexArray->Bind();
    }

    void VertexArray::Unbind() const
    {
        m_VertexArray->Unbind();
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        m_VertexArray->AddVertexBuffer(vertexBuffer->GetRHI(), vertexBuffer->GetLayout());
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        m_VertexArray->SetIndexBuffer(indexBuffer->GetRHI());
        m_IndexBuffer = indexBuffer;
    }

    const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }
}
