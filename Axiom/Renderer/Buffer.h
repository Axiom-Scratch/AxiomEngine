#pragma once

#include "RHI/RHIBuffer.h"
#include "RHI/RHIVertexArray.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace Axiom
{
    class VertexArray;

    class VertexBuffer
    {
    public:
        static std::shared_ptr<VertexBuffer> Create(const float* vertices, uint32_t size);

        void Bind() const;
        void Unbind() const;

        void SetLayout(const BufferLayout& layout);
        [[nodiscard]] const BufferLayout& GetLayout() const;

    private:
        explicit VertexBuffer(std::unique_ptr<RHIVertexBuffer> buffer);

        RHIVertexBuffer& GetRHI();
        const RHIVertexBuffer& GetRHI() const;

        std::unique_ptr<RHIVertexBuffer> m_Buffer;
        BufferLayout m_Layout;

        friend class VertexArray;
    };

    class IndexBuffer
    {
    public:
        static std::shared_ptr<IndexBuffer> Create(const uint32_t* indices, uint32_t count);

        void Bind() const;
        void Unbind() const;
        [[nodiscard]] uint32_t GetCount() const;

    private:
        explicit IndexBuffer(std::unique_ptr<RHIIndexBuffer> buffer);

        RHIIndexBuffer& GetRHI();
        const RHIIndexBuffer& GetRHI() const;

        std::unique_ptr<RHIIndexBuffer> m_Buffer;

        friend class VertexArray;
    };

    class VertexArray
    {
    public:
        static std::shared_ptr<VertexArray> Create();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
        [[nodiscard]] const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

    private:
        explicit VertexArray(std::unique_ptr<RHIVertexArray> vertexArray);

        std::unique_ptr<RHIVertexArray> m_VertexArray;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}
