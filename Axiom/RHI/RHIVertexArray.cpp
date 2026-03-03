#include "RHI/RHIVertexArray.h"

#include "RHI/OpenGL/OpenGLVertexArray.h"

namespace Axiom
{
    std::unique_ptr<RHIVertexArray> RHIVertexArray::Create()
    {
        return std::make_unique<OpenGLVertexArray>();
    }
}
