#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axiom
{
    Camera::Camera(float fov, float aspect, float nearClip, float farClip)
        : m_Projection(glm::perspective(glm::radians(fov), aspect, nearClip, farClip))
        , m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
    {
    }

    const glm::mat4& Camera::GetProjection() const
    {
        return m_Projection;
    }

    const glm::mat4& Camera::GetView() const
    {
        return m_View;
    }

    glm::mat4 Camera::GetViewProjection() const
    {
        return m_Projection * m_View;
    }
}
