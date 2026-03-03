#include "Axiom.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>

class SandboxApp final : public Axiom::Application
{
public:
    SandboxApp()
        : m_LastTime(std::chrono::steady_clock::now())
    {
        m_CameraEntity = m_Scene.CreateEntity("MainCamera");
        m_CameraEntity.AddComponent<Axiom::CameraComponent>();
        auto& cameraTransform = m_CameraEntity.GetComponent<Axiom::TransformComponent>();
        cameraTransform.Translation = Axiom::Math::Vec3(0.0f, 0.0f, 5.0f);
        cameraTransform.Rotation = Axiom::Math::Vec3(0.0f, -90.0f, 0.0f);

        auto helmet1 = Axiom::AssetManager::LoadModel("Assets/Models/DamagedHelmet.glb");
        auto helmet2 = Axiom::AssetManager::LoadModel("Assets/Models/DamagedHelmet2.glb");

        if (!helmet1 || helmet1->GetSubmeshes().empty())
        {
            Axiom::Log::CoreError("Failed to load model: Assets/Models/DamagedHelmet.glb");
        }
        if (!helmet2 || helmet2->GetSubmeshes().empty())
        {
            Axiom::Log::CoreError("Failed to load model: Assets/Models/DamagedHelmet2.glb");
        }

        auto e1 = m_Scene.CreateEntity("HelmetOne");
        if (helmet1)
        {
            e1.AddComponent<Axiom::ModelComponent>(helmet1);
        }
        e1.GetComponent<Axiom::TransformComponent>().Translation = Axiom::Math::Vec3(-2.0f, 0.0f, 0.0f);

        auto e2 = m_Scene.CreateEntity("HelmetTwo");
        if (helmet2)
        {
            e2.AddComponent<Axiom::ModelComponent>(helmet2);
        }
        e2.GetComponent<Axiom::TransformComponent>().Translation = Axiom::Math::Vec3(2.0f, 0.0f, 0.0f);
    }

protected:
    void OnUpdate() override
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> delta = now - m_LastTime;
        m_LastTime = now;

        const float timestep = delta.count();
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

        UpdateCamera(timestep);

        m_Scene.OnUpdate(timestep);
        m_Scene.OnRender(m_SceneRenderer);
    }

private:
    static Axiom::Math::Vec3 GetForward(const Axiom::Math::Vec3& rotation)
    {
        const float yawRadians = glm::radians(rotation.y);
        const float pitchRadians = glm::radians(rotation.x);

        Axiom::Math::Vec3 direction;
        direction.x = std::cos(yawRadians) * std::cos(pitchRadians);
        direction.y = std::sin(pitchRadians);
        direction.z = std::sin(yawRadians) * std::cos(pitchRadians);
        return glm::normalize(direction);
    }

    void UpdateCamera(float timestep)
    {
        auto& cameraTransform = m_CameraEntity.GetComponent<Axiom::TransformComponent>();

        const float speed = 5.0f * timestep;
        const auto forward = GetForward(cameraTransform.Rotation);
        const auto right = glm::normalize(glm::cross(forward, Axiom::Math::Vec3(0.0f, 1.0f, 0.0f)));

        if (Axiom::Input::IsKeyPressed(Axiom::Key::W))
        {
            cameraTransform.Translation += forward * speed;
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::S))
        {
            cameraTransform.Translation -= forward * speed;
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::A))
        {
            cameraTransform.Translation -= right * speed;
        }
        if (Axiom::Input::IsKeyPressed(Axiom::Key::D))
        {
            cameraTransform.Translation += right * speed;
        }

        const float sensitivity = 0.1f;
        const float deltaX = Axiom::Input::GetMouseDeltaX();
        const float deltaY = Axiom::Input::GetMouseDeltaY();
        cameraTransform.Rotation.y += deltaX * sensitivity;
        cameraTransform.Rotation.x = std::clamp(cameraTransform.Rotation.x - deltaY * sensitivity, -89.0f, 89.0f);
    }

    Axiom::Scene m_Scene;
    Axiom::SceneRenderer m_SceneRenderer;
    Axiom::Entity m_CameraEntity;
    std::chrono::steady_clock::time_point m_LastTime;
    bool m_EscapeWasDown = false;
};

Axiom::Scope<Axiom::Application> Axiom::CreateApplication()
{
    return Axiom::CreateScope<SandboxApp>();
}

#include "EntryPoint.h"
