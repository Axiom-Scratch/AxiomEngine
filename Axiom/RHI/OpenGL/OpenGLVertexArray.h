#pragma once

#include "RHI/RHIVertexArray.h"

#include <cstdint>

namespace Axiom
{
    class OpenGLVertexArray final : public RHIVertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void Bind() const override;
        void Unbind() const override;
        void AddVertexBuffer(const RHIVertexBuffer& vertexBuffer, const BufferLayout& layout) override;
        void SetIndexBuffer(const RHIIndexBuffer& indexBuffer) override;

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_VertexBufferIndex = 0;
    };
}
