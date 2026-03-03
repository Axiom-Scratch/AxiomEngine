#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string>

namespace Axiom
{
    class Texture2D;

    class Material
    {
    public:
        static std::shared_ptr<Material> Create(const std::string& vertexSource, const std::string& fragmentSource);

        ~Material();

        void Bind() const;
        void SetMat4(const std::string& name, const glm::mat4& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetInt(const std::string& name, int value);
        void SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture);

    private:
        struct MaterialData;

        Material(const std::string& vertexSource, const std::string& fragmentSource);

        std::unique_ptr<MaterialData> m_Data;
    };
}
