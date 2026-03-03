#pragma once

#include <cstdint>
#include <glm/vec4.hpp>

namespace Axiom
{
    class RenderCommand
    {
    public:
        static void Init();
        static void Clear(const glm::vec4& color);
        static void DrawIndexed(uint32_t count);
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    };
}
