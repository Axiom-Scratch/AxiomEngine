#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Math/Math.h"

namespace Axiom
{
    class Material;
    class Mesh;
    class ModelLoader;

    class Model
    {
    public:
        struct Submesh
        {
            std::shared_ptr<Mesh> MeshPtr;
            std::shared_ptr<Material> MaterialPtr;
            Math::Mat4 Transform = Math::Identity();
        };

        Model() = default;
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) = default;
        Model& operator=(Model&&) = default;

        std::vector<Submesh>& GetSubmeshes();
        [[nodiscard]] const std::vector<Submesh>& GetSubmeshes() const;

    private:
        friend class ModelLoader;

        std::vector<Submesh> m_Submeshes;
    };
}
