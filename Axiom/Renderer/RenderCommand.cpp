#include "Renderer/RenderCommand.h"

#include "RHI/RHI.h"

#include <memory>

namespace Axiom
{
    namespace
    {
        std::unique_ptr<RHI> s_RHI;
    }

    void RenderCommand::Init()
    {
        s_RHI = RHI::Create();
        s_RHI->Init();
    }

    void RenderCommand::Clear(float r, float g, float b, float a)
    {
        s_RHI->Clear(r, g, b, a);
    }

    void RenderCommand::DrawIndexed(uint32_t count)
    {
        s_RHI->DrawIndexed(count);
    }
}
