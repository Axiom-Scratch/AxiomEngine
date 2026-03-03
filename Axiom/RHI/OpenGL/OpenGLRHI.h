#pragma once

#include "RHI/RHI.h"

namespace Axiom
{
    class OpenGLRHI final : public RHI
    {
    public:
        void Init() override;
        void Clear(float r, float g, float b, float a) override;
        void DrawIndexed(uint32_t count) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    };
}
