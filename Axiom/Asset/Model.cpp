#include "Asset/Model.h"

#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/SceneRenderer.h"

namespace Axiom
{
    namespace
    {
        constexpr float DefaultFov = 45.0f;
        constexpr float DefaultAspect = 16.0f / 9.0f;
        constexpr float DefaultNearClip = 0.1f;
        constexpr float DefaultFarClip = 100.0f;
    }

    Model::Model()
        : m_Camera(std::make_unique<Camera>(DefaultFov, DefaultAspect, DefaultNearClip, DefaultFarClip))
    {
    }

    void Model::Draw()
    {
        if (!m_Camera)
        {
            m_Camera = std::make_unique<Camera>(DefaultFov, DefaultAspect, DefaultNearClip, DefaultFarClip);
        }

        SceneRenderer sceneRenderer;
        sceneRenderer.BeginScene(*m_Camera);

        if (m_Materials.empty())
        {
            sceneRenderer.EndScene();
            return;
        }

        for (const auto& submesh : m_Submeshes)
        {
            if (!submesh.MeshPtr)
            {
                continue;
            }

            const uint32_t materialIndex = submesh.MaterialIndex < m_Materials.size() ? submesh.MaterialIndex : 0u;
            auto& modelMaterial = m_Materials[materialIndex];
            if (modelMaterial.MaterialPtr)
            {
                modelMaterial.MaterialPtr->Bind();
                modelMaterial.MaterialPtr->SetVec4("u_BaseColor", modelMaterial.BaseColor);
                sceneRenderer.Submit(submesh.MeshPtr, modelMaterial.MaterialPtr, m_Transform * submesh.Transform);
            }
        }

        sceneRenderer.EndScene();
    }

    void Model::SetCamera(const Camera& camera)
    {
        m_Camera = std::make_unique<Camera>(camera);
    }

    void Model::SetTransform(const glm::mat4& transform)
    {
        m_Transform = transform;
    }

    const std::vector<Model::Submesh>& Model::GetSubmeshes() const
    {
        return m_Submeshes;
    }

    const std::vector<ModelMaterial>& Model::GetMaterials() const
    {
        return m_Materials;
    }
}
