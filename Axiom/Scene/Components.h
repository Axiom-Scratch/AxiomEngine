#pragma once

#include "Core/UUID.h"
#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <string>

namespace Axiom
{
    class Mesh;
    class Model;

    struct IDComponent
    {
        UUID ID;
    };

    struct TagComponent
    {
        std::string Tag;
    };

    struct TransformComponent
    {
        glm::vec3 Translation{ 0.0f };
        glm::vec3 Rotation{ 0.0f };
        glm::vec3 Scale{ 1.0f };

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation = glm::rotate(rotation, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(rotation, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(rotation, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            const glm::mat4 transform = glm::translate(glm::mat4(1.0f), Translation) * rotation;
            return glm::scale(transform, Scale);
        }
    };

    struct MeshComponent
    {
        std::shared_ptr<Mesh> MeshPtr;
    };

    struct ModelComponent
    {
        std::shared_ptr<Model> ModelPtr;
    };

    struct CameraComponent
    {
        Camera CameraData{ 45.0f, 1280.0f / 720.0f, 0.1f, 100.0f };
        bool Primary = true;
    };
}
