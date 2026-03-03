#pragma once

#include <cstdint>

namespace Axiom
{
    class RenderCommand
    {
    public:
        static void Init();
        static void Clear(float r, float g, float b, float a);
        static void DrawIndexed(uint32_t count);
    };
}
