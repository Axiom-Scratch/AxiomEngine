#include "Renderer/Camera.h"

#include <algorithm>
#include <cmath>

namespace Axiom
{
    Camera::Camera(float fov, float aspect, float nearClip, float farClip)
    {
        m_Projection = Math::Perspective(fov, aspect, nearClip, farClip);
        UpdateView();
    }

    void Camera::SetPosition(const Math::Vec3& pos)
    {
        m_Position = pos;
        UpdateView();
    }

    void Camera::SetRotation(float yaw, float pitch)
    {
        m_Yaw = yaw;
        m_Pitch = std::clamp(pitch, -89.0f, 89.0f);
        UpdateView();
    }

    void Camera::MoveForward(float amount)
    {
        m_Position += m_Forward * amount;
        UpdateView();
    }

    void Camera::MoveRight(float amount)
    {
        const Math::Vec3 right = glm::normalize(glm::cross(m_Forward, m_Up));
        m_Position += right * amount;
        UpdateView();
    }

    void Camera::AddRotation(float yawDelta, float pitchDelta)
    {
        m_Yaw += yawDelta;
        m_Pitch = std::clamp(m_Pitch + pitchDelta, -89.0f, 89.0f);
        UpdateView();
    }

    void Camera::UpdateView()
    {
        const float yawRadians = glm::radians(m_Yaw);
        const float pitchRadians = glm::radians(m_Pitch);

        Math::Vec3 direction;
        direction.x = std::cos(yawRadians) * std::cos(pitchRadians);
        direction.y = std::sin(pitchRadians);
        direction.z = std::sin(yawRadians) * std::cos(pitchRadians);
        m_Forward = glm::normalize(direction);

        const Math::Vec3 right = glm::normalize(glm::cross(m_Forward, Math::Vec3(0.0f, 1.0f, 0.0f)));
        m_Up = glm::normalize(glm::cross(right, m_Forward));

        m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    }

    const Math::Mat4& Camera::GetViewMatrix() const
    {
        return m_View;
    }

    const Math::Mat4& Camera::GetProjectionMatrix() const
    {
        return m_Projection;
    }
}
