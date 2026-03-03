#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "Renderer/Camera.h"

namespace Axiom
{
    class Material;
    class Mesh;
    class ModelLoader;

    struct ModelTexture
    {
        enum class Type
        {
            Diffuse,
            Specular,
            Normal,
            Emissive,
            Unknown
        };

        std::string Path;
        Type TextureType = Type::Unknown;
        bool Embedded = false;
    };

    struct ModelMaterial
    {
        glm::vec4 BaseColor = glm::vec4(1.0f);
        std::vector<ModelTexture> Textures;
        std::shared_ptr<Material> MaterialPtr;
    };

    class Model
    {
    public:
        struct Submesh
        {
            std::shared_ptr<Mesh> MeshPtr;
            uint32_t MaterialIndex = 0;
            glm::mat4 Transform = glm::mat4(1.0f);
        };

        Model();
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) = default;
        Model& operator=(Model&&) = default;

        void Draw();
        void SetCamera(const Camera& camera);
        void SetTransform(const glm::mat4& transform);

        [[nodiscard]] const std::vector<Submesh>& GetSubmeshes() const;
        [[nodiscard]] const std::vector<ModelMaterial>& GetMaterials() const;

    private:
        friend class ModelLoader;

        std::vector<Submesh> m_Submeshes;
        std::vector<ModelMaterial> m_Materials;
        std::unique_ptr<Camera> m_Camera;
        glm::mat4 m_Transform{ 1.0f };
    };
}
