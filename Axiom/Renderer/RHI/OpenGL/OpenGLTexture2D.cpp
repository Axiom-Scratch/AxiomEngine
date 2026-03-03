#include "Renderer/RHI/OpenGL/OpenGLTexture2D.h"

#include "Core/Log.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cstddef>
#include <memory>

namespace Axiom
{
    namespace
    {
        struct StbiDeleter
        {
            void operator()(unsigned char* data) const
            {
                stbi_image_free(data);
            }
        };

        using StbiImagePtr = std::unique_ptr<unsigned char, StbiDeleter>;

        struct ImageData
        {
            StbiImagePtr Pixels;
            int Width = 0;
            int Height = 0;
            int Channels = 0;
        };

        ImageData LoadImageFromFile(const std::string& path)
        {
            ImageData image;
            image.Pixels.reset(stbi_load(path.c_str(), &image.Width, &image.Height, &image.Channels, 0));
            return image;
        }

        ImageData LoadImageFromMemory(const void* data, uint32_t size)
        {
            ImageData image;
            image.Pixels.reset(stbi_load_from_memory(static_cast<const stbi_uc*>(data),
                static_cast<int>(size), &image.Width, &image.Height, &image.Channels, 0));
            return image;
        }

        GLenum ToDataFormat(int channels)
        {
            switch (channels)
            {
                case 1: return GL_RED;
                case 2: return GL_RG;
                case 3: return GL_RGB;
                case 4: return GL_RGBA;
                default: return GL_RGBA;
            }
        }

        GLenum ToInternalFormat(int channels, bool srgb)
        {
            if (!srgb)
            {
                switch (channels)
                {
                    case 1: return GL_R8;
                    case 2: return GL_RG8;
                    case 3: return GL_RGB8;
                    case 4: return GL_RGBA8;
                    default: return GL_RGBA8;
                }
            }

            switch (channels)
            {
                case 3: return GL_SRGB8;
                case 4: return GL_SRGB8_ALPHA8;
                default: return ToInternalFormat(channels, false);
            }
        }

        uint32_t CreateTextureFromPixels(const void* data,
            int width,
            int height,
            int channels,
            bool srgb)
        {
            if (!data || width <= 0 || height <= 0)
            {
                return 0;
            }

            const GLenum dataFormat = ToDataFormat(channels);
            const GLenum internalFormat = ToInternalFormat(channels, srgb);

            uint32_t rendererId = 0;
            glGenTextures(1, &rendererId);
            glBindTexture(GL_TEXTURE_2D, rendererId);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internalFormat),
                width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

            return rendererId;
        }
    }

    std::shared_ptr<OpenGLTexture2D> OpenGLTexture2D::CreateFromFile(const std::string& path)
    {
        ImageData image = LoadImageFromFile(path);
        if (!image.Pixels)
        {
            Log::CoreError("Failed to load texture from file: {}", path);
            return nullptr;
        }

        const uint32_t rendererId = CreateTextureFromPixels(image.Pixels.get(),
            image.Width, image.Height, image.Channels, true);
        if (rendererId == 0)
        {
            return nullptr;
        }

        return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(static_cast<uint32_t>(image.Width),
            static_cast<uint32_t>(image.Height), rendererId));
    }

    std::shared_ptr<OpenGLTexture2D> OpenGLTexture2D::CreateFromMemory(const void* data, uint32_t size)
    {
        ImageData image = LoadImageFromMemory(data, size);
        if (!image.Pixels)
        {
            Log::CoreError("Failed to load texture from memory.");
            return nullptr;
        }

        const uint32_t rendererId = CreateTextureFromPixels(image.Pixels.get(),
            image.Width, image.Height, image.Channels, true);
        if (rendererId == 0)
        {
            return nullptr;
        }

        return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(static_cast<uint32_t>(image.Width),
            static_cast<uint32_t>(image.Height), rendererId));
    }

    std::shared_ptr<OpenGLTexture2D> OpenGLTexture2D::CreateFromPixels(const void* data,
        uint32_t width,
        uint32_t height,
        bool srgb)
    {
        const uint32_t rendererId = CreateTextureFromPixels(data,
            static_cast<int>(width), static_cast<int>(height), 4, srgb);
        if (rendererId == 0)
        {
            return nullptr;
        }

        return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(width, height, rendererId));
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t rendererId)
        : m_RendererID(rendererId)
        , m_Width(width)
        , m_Height(height)
    {
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        if (m_RendererID != 0)
        {
            glDeleteTextures(1, &m_RendererID);
        }
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
}
