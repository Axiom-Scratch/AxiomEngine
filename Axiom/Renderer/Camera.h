#pragma once

#include "Math/Math.h"

namespace Axiom
{
    class Camera
    {
    public:
        Camera(float fov, float aspect, float nearClip, float farClip);

        void SetPosition(const Math::Vec3& pos);
        void SetRotation(float yaw, float pitch);
        void MoveForward(float amount);
        void MoveRight(float amount);
        void AddRotation(float yawDelta, float pitchDelta);
        void UpdateView();

        [[nodiscard]] const Math::Mat4& GetViewMatrix() const;
        [[nodiscard]] const Math::Mat4& GetProjectionMatrix() const;

    private:
        Math::Vec3 m_Position{ 0.0f, 0.0f, 3.0f };
        Math::Vec3 m_Forward{ 0.0f, 0.0f, -1.0f };
        Math::Vec3 m_Up{ 0.0f, 1.0f, 0.0f };
        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        Math::Mat4 m_View = Math::Identity();
        Math::Mat4 m_Projection = Math::Identity();
    };
}
