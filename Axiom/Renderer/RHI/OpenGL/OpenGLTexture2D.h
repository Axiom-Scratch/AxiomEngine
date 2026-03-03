#pragma once

#include "Renderer/Texture2D.h"

#include <cstdint>
#include <memory>
#include <string>

namespace Axiom
{
    class OpenGLTexture2D final : public Texture2D
    {
    public:
        static std::shared_ptr<OpenGLTexture2D> CreateFromFile(const std::string& path);
        static std::shared_ptr<OpenGLTexture2D> CreateFromMemory(const void* data, uint32_t size);
        static std::shared_ptr<OpenGLTexture2D> CreateFromPixels(const void* data, uint32_t width, uint32_t height, bool srgb);

        ~OpenGLTexture2D() override;

        void Bind(uint32_t slot = 0) const override;

    private:
        OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t rendererId);

        uint32_t m_RendererID = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}
