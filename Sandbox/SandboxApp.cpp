#include "Axiom.h"

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

class SandboxApp final : public Axiom::Application
{
public:
    SandboxApp()
        : m_LastTime(std::chrono::steady_clock::now())
    {
        const float aspect = 1280.0f / 720.0f;
        m_Camera = std::make_unique<Axiom::Camera>(45.0f, aspect, 0.1f, 100.0f);
        m_Model = Axiom::AssetManager::LoadModel("Assets/Models/DamagedHelmet.glb");
        m_ModelLoaded = m_Model && !m_Model->GetSubmeshes().empty();
        if (!m_ModelLoaded)
        {
            Axiom::Log::CoreError("Failed to load model: Assets/Models/DamagedHelmet.glb");
            return;
        }

        CacheOriginalMaterials();
        BuildCustomMaterial();
    }

protected:
    void OnUpdate() override
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> delta = now - m_LastTime;
        m_LastTime = now;

        const float timestep = delta.count();
        const float speed = 5.0f * timestep;
        auto* window = static_cast<GLFWwindow*>(GetWindow().GetNativeHandle());

        if (Axiom::Input::IsKeyPressed(Axiom::Key::Escape))
        {
            if (!m_EscapeWasDown)
            {
                Axiom::ToggleCursor(window);
            }
            m_EscapeWasDown = true;
        }
        else
        {
            m_EscapeWasDown = false;
        }

        if (Axiom::Input::IsKeyPressed(Axiom::Key::W))
        {
            m_Camera->MoveForward(speed);
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::S))
        {
            m_Camera->MoveForward(-speed);
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::A))
        {
            m_Camera->MoveRight(-speed);
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::D))
        {
            m_Camera->MoveRight(speed);
        }

        const float sensitivity = 0.1f;
        const float deltaX = Axiom::Input::GetMouseDeltaX();
        const float deltaY = Axiom::Input::GetMouseDeltaY();
        m_Camera->AddRotation(deltaX * sensitivity, -deltaY * sensitivity);

        if (!m_ModelLoaded)
        {
            return;
        }

        m_Rotation += timestep;
        UpdateMaterialSwap(timestep);

        const Axiom::Math::Mat4 transform = Axiom::Math::Rotate(
            Axiom::Math::Identity(),
            m_Rotation,
            Axiom::Math::Vec3(0.0f, 1.0f, 0.0f));

        m_SceneRenderer.BeginScene(*m_Camera);

        for (const auto& submesh : m_Model->GetSubmeshes())
        {
            if (!submesh.MeshPtr || !submesh.MaterialPtr)
            {
                continue;
            }

            m_SceneRenderer.Submit(submesh.MeshPtr, submesh.MaterialPtr, transform * submesh.Transform);
        }

        m_SceneRenderer.EndScene();
    }

private:
    void CacheOriginalMaterials()
    {
        m_OriginalMaterials.clear();
        m_OriginalMaterials.reserve(m_Model->GetSubmeshes().size());

        for (const auto& submesh : m_Model->GetSubmeshes())
        {
            m_OriginalMaterials.push_back(submesh.MaterialPtr);
        }
    }

    void BuildCustomMaterial()
    {
        const std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}
)";

        const std::string fragmentSource = R"(
#version 330 core
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Albedo;
uniform vec4 u_BaseColor;

void main()
{
    color = texture(u_Albedo, v_TexCoord) * u_BaseColor;
}
)";

        m_CustomMaterial = Axiom::Material::Create(vertexSource, fragmentSource);
        if (!m_CustomMaterial)
        {
            Axiom::Log::CoreError("Failed to create custom material.");
            return;
        }

        const std::string texturePath = "Assets/Textures/Test.png";
        std::shared_ptr<Axiom::Texture2D> customTexture;
        if (std::filesystem::exists(texturePath))
        {
            customTexture = Axiom::AssetManager::LoadTexture(texturePath);
        }

        if (!customTexture)
        {
            const unsigned char whitePixel[4] = { 255, 255, 255, 255 };
            customTexture = Axiom::Texture2D::CreateFromPixels(whitePixel, 1, 1, true);
        }

        m_CustomMaterial->SetTexture("u_Albedo", customTexture);
        m_CustomMaterial->SetVec4("u_BaseColor", Axiom::Math::Vec4(0.2f, 0.8f, 0.2f, 1.0f));
    }

    void UpdateMaterialSwap(float timestep)
    {
        if (!m_CustomMaterial || m_OriginalMaterials.empty())
        {
            return;
        }

        m_MaterialSwapTimer += timestep;
        if (m_MaterialSwapTimer < m_MaterialSwapInterval)
        {
            return;
        }

        m_MaterialSwapTimer = 0.0f;
        m_UseCustomMaterial = !m_UseCustomMaterial;
        ApplyMaterialOverride();
    }

    void ApplyMaterialOverride()
    {
        auto& submeshes = m_Model->GetSubmeshes();
        if (submeshes.size() != m_OriginalMaterials.size())
        {
            return;
        }

        for (size_t i = 0; i < submeshes.size(); ++i)
        {
            submeshes[i].MaterialPtr = m_UseCustomMaterial ? m_CustomMaterial : m_OriginalMaterials[i];
        }
    }

    std::unique_ptr<Axiom::Camera> m_Camera;
    std::shared_ptr<Axiom::Model> m_Model;
    bool m_ModelLoaded = false;
    Axiom::SceneRenderer m_SceneRenderer;
    std::chrono::steady_clock::time_point m_LastTime;
    float m_Rotation = 0.0f;
    float m_MaterialSwapTimer = 0.0f;
    float m_MaterialSwapInterval = 2.0f;
    bool m_UseCustomMaterial = false;
    bool m_EscapeWasDown = false;
    std::vector<std::shared_ptr<Axiom::Material>> m_OriginalMaterials;
    std::shared_ptr<Axiom::Material> m_CustomMaterial;
};

Axiom::Scope<Axiom::Application> Axiom::CreateApplication()
{
    return Axiom::CreateScope<SandboxApp>();
}

#include "EntryPoint.h"
