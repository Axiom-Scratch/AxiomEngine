#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Axiom
{
    class Mesh
    {
    public:
        struct Vertex
        {
            glm::vec3 Position{};
            glm::vec3 Normal{};
            glm::vec2 TexCoord{};
        };

        static std::shared_ptr<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        static std::shared_ptr<Mesh> CreateCube();

        ~Mesh();

        void Draw() const;

    private:
        struct MeshData;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        std::unique_ptr<MeshData> m_Data;
    };
}
