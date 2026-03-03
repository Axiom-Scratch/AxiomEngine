#include "Renderer/RenderCommand.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/RHI/RHI.h"

#include <memory>

namespace Axiom
{
    namespace
    {
        std::unique_ptr<RendererAPI> s_RendererAPI;
        GLProcAddress s_GLLoader = nullptr;
    }

    void RenderCommand::SetGLLoader(GLProcAddress loader)
    {
        s_GLLoader = loader;
    }

    void RenderCommand::Init()
    {
        s_RendererAPI = RendererAPI::Create(RHI::GetAPIType());
        if (s_RendererAPI)
        {
            s_RendererAPI->SetGLLoader(s_GLLoader);
        }
        s_RendererAPI->Init();
    }

    void RenderCommand::Clear(const glm::vec4& color)
    {
        s_RendererAPI->Clear(color);
    }

    void RenderCommand::DrawIndexed(uint32_t count)
    {
        s_RendererAPI->DrawIndexed(count);
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }
}
