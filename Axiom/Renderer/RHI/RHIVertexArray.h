#pragma once

#include <memory>

namespace Axiom
{
    class BufferLayout;
    class RHIVertexBuffer;
    class RHIIndexBuffer;

    class RHIVertexArray
    {
    public:
        virtual ~RHIVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void AddVertexBuffer(const RHIVertexBuffer& vertexBuffer, const BufferLayout& layout) = 0;
        virtual void SetIndexBuffer(const RHIIndexBuffer& indexBuffer) = 0;

        static std::unique_ptr<RHIVertexArray> Create();
    };
}
