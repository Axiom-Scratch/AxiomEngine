#include "Renderer/Material.h"

#include "Renderer/RHI/RHIShader.h"
#include "Renderer/Texture2D.h"

namespace Axiom
{
    struct Material::MaterialData
    {
        std::unique_ptr<RHIShader> Shader;
        std::shared_ptr<Texture2D> AlbedoTexture;
    };

    std::shared_ptr<Material> Material::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        return std::shared_ptr<Material>(new Material(vertexSource, fragmentSource));
    }

    Material::Material(const std::string& vertexSource, const std::string& fragmentSource)
        : m_Data(std::make_unique<MaterialData>())
    {
        m_Data->Shader = RHIShader::Create(vertexSource, fragmentSource);
    }

    Material::~Material() = default;

    void Material::Bind() const
    {
        m_Data->Shader->Bind();
        if (m_Data->AlbedoTexture)
        {
            m_Data->AlbedoTexture->Bind(0);
            m_Data->Shader->SetInt("u_Albedo", 0);
        }
    }

    void Material::SetMat4(const std::string& name, const glm::mat4& value)
    {
        m_Data->Shader->SetMat4(name, value);
    }

    void Material::SetVec4(const std::string& name, const glm::vec4& value)
    {
        m_Data->Shader->SetVec4(name, value);
    }

    void Material::SetInt(const std::string& name, int value)
    {
        m_Data->Shader->SetInt(name, value);
    }

    void Material::SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture)
    {
        m_Data->AlbedoTexture = texture;
    }
}
