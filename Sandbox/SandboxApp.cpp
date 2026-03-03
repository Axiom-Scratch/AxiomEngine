#include "Axiom.h"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class SandboxApp final : public Axiom::Application
{
public:
    SandboxApp()
        : m_LastTime(std::chrono::steady_clock::now())
    {
        const float aspect = 1280.0f / 720.0f;
        m_Camera = std::make_unique<Axiom::Camera>(45.0f, aspect, 0.1f, 100.0f);
        m_Model = Axiom::ModelLoader::Load("Assets/Models/DamagedHelmet.glb");
        m_ModelLoaded = !m_Model.GetSubmeshes().empty();
        if (!m_ModelLoaded)
        {
            Axiom::Log::CoreError("Failed to load model: Assets/Models/DamagedHelmet.glb");
        }
    }

protected:
    void OnUpdate() override
    {
        if (!m_ModelLoaded)
        {
            return;
        }

        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> delta = now - m_LastTime;
        m_LastTime = now;

        const float timestep = delta.count();
        m_Rotation += timestep;

        const glm::mat4 transform = glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));

        m_SceneRenderer.BeginScene(*m_Camera);

        const auto& materials = m_Model.GetMaterials();
        if (materials.empty())
        {
            m_SceneRenderer.EndScene();
            return;
        }

        for (const auto& submesh : m_Model.GetSubmeshes())
        {
            if (!submesh.MeshPtr)
            {
                continue;
            }

            const uint32_t materialIndex = submesh.MaterialIndex < materials.size() ? submesh.MaterialIndex : 0u;
            const auto& modelMaterial = materials[materialIndex];
            if (!modelMaterial.MaterialPtr)
            {
                continue;
            }

            modelMaterial.MaterialPtr->Bind();
            modelMaterial.MaterialPtr->SetVec4("u_BaseColor", modelMaterial.BaseColor);
            m_SceneRenderer.Submit(submesh.MeshPtr, modelMaterial.MaterialPtr, transform * submesh.Transform);
        }

        m_SceneRenderer.EndScene();
    }

private:
    std::unique_ptr<Axiom::Camera> m_Camera;
    Axiom::Model m_Model;
    bool m_ModelLoaded = false;
    Axiom::SceneRenderer m_SceneRenderer;
    std::chrono::steady_clock::time_point m_LastTime;
    float m_Rotation = 0.0f;
};

Axiom::Scope<Axiom::Application> Axiom::CreateApplication()
{
    return Axiom::CreateScope<SandboxApp>();
}

#include "EntryPoint.h"
