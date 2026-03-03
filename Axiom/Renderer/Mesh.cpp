#include "Renderer/Mesh.h"

#include "Renderer/RHI/RHIBuffer.h"
#include "Renderer/RHI/RHIVertexArray.h"

#include <type_traits>

namespace Axiom
{
    struct Mesh::MeshData
    {
        std::unique_ptr<RHIVertexArray> VertexArray;
        std::unique_ptr<RHIVertexBuffer> VertexBuffer;
        std::unique_ptr<RHIIndexBuffer> IndexBuffer;
        uint32_t IndexCount = 0;
    };

    std::shared_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        return std::shared_ptr<Mesh>(new Mesh(vertices, indices));
    }

    std::shared_ptr<Mesh> Mesh::CreateCube()
    {
        const float n = 0.57735026f;
        const std::vector<Vertex> vertices = {
            { { -0.5f, -0.5f, -0.5f }, { -n, -n, -n }, { 0.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, { n, -n, -n }, { 0.0f, 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, { n, n, -n }, { 0.0f, 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, { -n, n, -n }, { 0.0f, 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, { -n, -n, n }, { 0.0f, 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, { n, -n, n }, { 0.0f, 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, { n, n, n }, { 0.0f, 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, { -n, n, n }, { 0.0f, 0.0f } }
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            4, 5, 1, 1, 0, 4,
            7, 6, 2, 2, 3, 7,
            5, 6, 2, 2, 1, 5,
            4, 7, 3, 3, 0, 4
        };

        return Create(vertices, indices);
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : m_Data(std::make_unique<MeshData>())
    {
        static_assert(std::is_standard_layout_v<Vertex>, "Mesh::Vertex must be standard layout.");
        static_assert(sizeof(Vertex) == sizeof(float) * 8, "Mesh::Vertex must be tightly packed.");

        m_Data->VertexArray = RHIVertexArray::Create();
        m_Data->VertexBuffer = RHIVertexBuffer::Create(reinterpret_cast<const float*>(vertices.data()),
            static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };

        m_Data->VertexArray->AddVertexBuffer(*m_Data->VertexBuffer, layout);
        m_Data->IndexBuffer = RHIIndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
        m_Data->VertexArray->SetIndexBuffer(*m_Data->IndexBuffer);
        m_Data->IndexCount = static_cast<uint32_t>(indices.size());
    }

    Mesh::~Mesh() = default;

    void Mesh::Bind() const
    {
        m_Data->VertexArray->Bind();
    }

    uint32_t Mesh::GetIndexCount() const
    {
        return m_Data->IndexCount;
    }
}
