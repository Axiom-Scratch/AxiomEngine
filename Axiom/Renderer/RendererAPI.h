#pragma once

#include <cstdint>
#include <glm/vec4.hpp>

#include <memory>

namespace Axiom
{
    enum class RendererAPIType
    {
        None = 0,
        OpenGL = 1
    };

    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void Clear(const glm::vec4& color) = 0;
        virtual void DrawIndexed(uint32_t indexCount) = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        static std::unique_ptr<RendererAPI> Create(RendererAPIType type);
    };
}
