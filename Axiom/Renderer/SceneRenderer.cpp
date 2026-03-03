#include "Renderer/SceneRenderer.h"

#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderCommand.h"

namespace Axiom
{
    void SceneRenderer::BeginScene(const Camera& camera)
    {
        m_FrameData.ViewProjection = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        RenderCommand::Clear(Math::Vec4(0.08f, 0.08f, 0.1f, 1.0f));
    }

    void SceneRenderer::Submit(const std::shared_ptr<Mesh>& mesh,
        const std::shared_ptr<Material>& material,
        const Math::Mat4& transform)
    {
        if (!mesh || !material)
        {
            return;
        }

        material->Bind();
        material->SetMat4("u_MVP", m_FrameData.ViewProjection * transform);
        mesh->Draw();
    }

    void SceneRenderer::EndScene()
    {
    }
}
