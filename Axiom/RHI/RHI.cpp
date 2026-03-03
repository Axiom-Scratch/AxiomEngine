#include "RHI/RHI.h"

#include "RHI/OpenGL/OpenGLRHI.h"

namespace Axiom
{
    std::unique_ptr<RHI> RHI::Create()
    {
        return std::make_unique<OpenGLRHI>();
    }
}
