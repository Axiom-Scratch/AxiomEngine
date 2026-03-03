#pragma once

#include "RHI/RHIBuffer.h"

namespace Axiom
{
    class OpenGLVertexBuffer final : public RHIVertexBuffer
    {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        [[nodiscard]] uint32_t GetRendererID() const;

    private:
        uint32_t m_RendererID = 0;
    };

    class OpenGLIndexBuffer final : public RHIIndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;
        [[nodiscard]] uint32_t GetCount() const override;
        [[nodiscard]] uint32_t GetRendererID() const;

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_Count = 0;
    };
}
