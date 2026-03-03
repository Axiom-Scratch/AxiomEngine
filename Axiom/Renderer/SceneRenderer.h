#pragma once

#include "Renderer/FrameData.h"

#include <glm/mat4x4.hpp>

#include <memory>

namespace Axiom
{
    class Camera;
    class Material;
    class Mesh;

    class SceneRenderer
    {
    public:
        void BeginScene(const Camera& camera);
        void Submit(const std::shared_ptr<Mesh>& mesh,
            const std::shared_ptr<Material>& material,
            const glm::mat4& transform);
        void EndScene();

    private:
        FrameData m_FrameData{};
    };
}
