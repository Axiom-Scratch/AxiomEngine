#include "Renderer/Texture2D.h"

#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/OpenGL/OpenGLTexture2D.h"

namespace Axiom
{
    std::shared_ptr<Texture2D> Texture2D::CreateFromFile(const std::string& path)
    {
        switch (RHI::GetAPIType())
        {
            case RendererAPIType::OpenGL:
                return OpenGLTexture2D::CreateFromFile(path);
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::CreateFromMemory(const void* data, uint32_t size)
    {
        switch (RHI::GetAPIType())
        {
            case RendererAPIType::OpenGL:
                return OpenGLTexture2D::CreateFromMemory(data, size);
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::CreateFromPixels(const void* data, uint32_t width, uint32_t height, bool srgb)
    {
        switch (RHI::GetAPIType())
        {
            case RendererAPIType::OpenGL:
                return OpenGLTexture2D::CreateFromPixels(data, width, height, srgb);
            case RendererAPIType::None:
                return nullptr;
        }
        return nullptr;
    }
}
