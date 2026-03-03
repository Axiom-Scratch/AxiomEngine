#include "Renderer/Material.h"

#include "Renderer/RHI/RHIShader.h"
#include "Renderer/Texture2D.h"

#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>

namespace Axiom
{
    struct Material::MaterialData
    {
        std::unique_ptr<RHIShader> Shader;
        struct TextureBinding
        {
            std::string Name;
            std::shared_ptr<Texture2D> Texture;
            uint32_t Slot = 0;
        };

        std::vector<TextureBinding> Textures;
        std::unordered_map<std::string, glm::vec4> Vec4Parameters;
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

        for (const auto& binding : m_Data->Textures)
        {
            if (!binding.Texture)
            {
                continue;
            }

            binding.Texture->Bind(binding.Slot);
            m_Data->Shader->SetInt(binding.Name, static_cast<int>(binding.Slot));
        }

        for (const auto& [name, value] : m_Data->Vec4Parameters)
        {
            m_Data->Shader->SetVec4(name, value);
        }
    }

    void Material::SetMat4(const std::string& name, const glm::mat4& value)
    {
        m_Data->Shader->SetMat4(name, value);
    }

    void Material::SetVec4(const std::string& name, const glm::vec4& value)
    {
        m_Data->Vec4Parameters[name] = value;
    }

    void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture2D>& texture)
    {
        for (auto& binding : m_Data->Textures)
        {
            if (binding.Name == name)
            {
                binding.Texture = texture;
                return;
            }
        }

        MaterialData::TextureBinding binding;
        binding.Name = name;
        binding.Texture = texture;
        binding.Slot = static_cast<uint32_t>(m_Data->Textures.size());
        m_Data->Textures.push_back(std::move(binding));
    }
}
