#include "Renderer/SceneRenderer.h"

#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderCommand.h"

#include <glm/vec4.hpp>

namespace Axiom
{
    void SceneRenderer::BeginScene(const Camera& camera)
    {
        m_FrameData.ViewProjection = camera.GetViewProjection();
        RenderCommand::Clear(glm::vec4(0.08f, 0.08f, 0.1f, 1.0f));
    }

    void SceneRenderer::Submit(const std::shared_ptr<Mesh>& mesh,
        const std::shared_ptr<Material>& material,
        const glm::mat4& transform)
    {
        material->Bind();
        material->SetMat4("u_MVP", m_FrameData.ViewProjection * transform);

        mesh->Bind();
        RenderCommand::DrawIndexed(mesh->GetIndexCount());
    }

    void SceneRenderer::EndScene()
    {
    }
}
