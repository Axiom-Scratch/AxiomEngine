#include "RHI/RHIShader.h"

#include "RHI/OpenGL/OpenGLShader.h"

namespace Axiom
{
    std::unique_ptr<RHIShader> RHIShader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        return std::make_unique<OpenGLShader>(vertexSource, fragmentSource);
    }
}
