#pragma once

#include "Renderer/RendererAPI.h"

namespace Axiom
{
    class RHI
    {
    public:
        static RendererAPIType GetAPIType()
        {
            return RendererAPIType::OpenGL;
        }
    };
}
