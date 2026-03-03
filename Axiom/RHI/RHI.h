#pragma once

#include <cstdint>
#include <memory>

namespace Axiom
{
    class RHI
    {
    public:
        virtual ~RHI() = default;

        virtual void Init() = 0;
        virtual void Clear(float r, float g, float b, float a) = 0;
        virtual void DrawIndexed(uint32_t count) = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        static std::unique_ptr<RHI> Create();
    };
}
