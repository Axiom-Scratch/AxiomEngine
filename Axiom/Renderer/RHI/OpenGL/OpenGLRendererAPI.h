#pragma once

#include "Renderer/RendererAPI.h"

namespace Axiom
{
    class OpenGLRendererAPI final : public RendererAPI
    {
    public:
        void Init() override;
        void Clear(const glm::vec4& color) override;
        void DrawIndexed(uint32_t indexCount) override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    };
}
