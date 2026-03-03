#pragma once

#include <glm/mat4x4.hpp>

namespace Axiom
{
    class Camera
    {
    public:
        Camera(float fov, float aspect, float nearClip, float farClip);

        [[nodiscard]] const glm::mat4& GetProjection() const;
        [[nodiscard]] const glm::mat4& GetView() const;
        [[nodiscard]] glm::mat4 GetViewProjection() const;

    private:
        glm::mat4 m_Projection{1.0f};
        glm::mat4 m_View{1.0f};
    };
}
