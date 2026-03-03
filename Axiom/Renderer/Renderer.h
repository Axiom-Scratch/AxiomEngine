#pragma once

#include "Renderer/Camera.h"

#include <glm/mat4x4.hpp>

#include <memory>

namespace Axiom
{
    class Shader;
    class VertexArray;

    class Renderer
    {
    public:
        static void Init();
        static void BeginScene(const Camera& camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<VertexArray>& vertexArray,
            const glm::mat4& transform);
    };
}
