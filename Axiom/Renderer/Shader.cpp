#include "Renderer/Shader.h"

#include "RHI/RHIShader.h"

namespace Axiom
{
    std::shared_ptr<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        return std::shared_ptr<Shader>(new Shader(RHIShader::Create(vertexSource, fragmentSource)));
    }

    Shader::~Shader() = default;

    Shader::Shader(std::unique_ptr<RHIShader> shader)
        : m_Shader(std::move(shader))
    {
    }

    void Shader::Bind() const
    {
        m_Shader->Bind();
    }

    void Shader::Unbind() const
    {
        m_Shader->Unbind();
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        m_Shader->SetMat4(name, value);
    }
}
