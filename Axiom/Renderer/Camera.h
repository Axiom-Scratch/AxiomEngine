#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Axiom
{
    class Camera
    {
    public:
        Camera(float fov, float aspect, float nearClip, float farClip)
            : m_Projection(glm::perspective(glm::radians(fov), aspect, nearClip, farClip))
            , m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
        {
        }

        [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }
        [[nodiscard]] const glm::mat4& GetView() const { return m_View; }
        [[nodiscard]] glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

    private:
        glm::mat4 m_Projection{1.0f};
        glm::mat4 m_View{1.0f};
    };
}
