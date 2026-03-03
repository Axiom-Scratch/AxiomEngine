#include "Renderer/Renderer.h"

#include "Renderer/Buffer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"

#include <memory>

namespace Axiom
{
    namespace
    {
        struct SceneData
        {
            glm::mat4 ViewProjection{1.0f};
        };

        std::unique_ptr<SceneData> s_SceneData;
    }

    void Renderer::Init()
    {
        RenderCommand::Init();
        if (!s_SceneData)
        {
            s_SceneData = std::make_unique<SceneData>();
        }
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData->ViewProjection = camera.GetViewProjection();
        RenderCommand::Clear(0.1f, 0.1f, 0.12f, 1.0f);
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<VertexArray>& vertexArray,
        const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_MVP", s_SceneData->ViewProjection * transform);

        vertexArray->Bind();
        const auto& indexBuffer = vertexArray->GetIndexBuffer();
        RenderCommand::DrawIndexed(indexBuffer->GetCount());
    }
}
