#include "Renderer/SceneRenderer.h"

#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderCommand.h"

namespace Axiom
{
    namespace
    {
        std::shared_ptr<Material> CreateDefaultMaterial()
        {
            const std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}
)";

            const std::string fragmentSource = R"(
#version 330 core
out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
)";

            auto material = Material::Create(vertexSource, fragmentSource);
            if (material)
            {
                material->SetVec4("u_Color", Math::Vec4(1.0f));
            }
            return material;
        }
    }

    void SceneRenderer::BeginScene(const Camera& camera)
    {
        BeginScene(camera.GetProjectionMatrix() * camera.GetViewMatrix());
    }

    void SceneRenderer::BeginScene(const Math::Mat4& viewProjection)
    {
        m_FrameData.ViewProjection = viewProjection;
        RenderCommand::Clear(Math::Vec4(0.1f, 0.1f, 0.15f, 1.0f));
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
        material->SetMat4("u_ViewProjection", m_FrameData.ViewProjection);
        material->SetMat4("u_Model", transform);
        mesh->Draw();
    }

    void SceneRenderer::Submit(const std::shared_ptr<Mesh>& mesh,
        const Math::Mat4& transform)
    {
        if (!mesh)
        {
            return;
        }

        static std::shared_ptr<Material> s_DefaultMaterial = CreateDefaultMaterial();
        if (!s_DefaultMaterial)
        {
            return;
        }

        Submit(mesh, s_DefaultMaterial, transform);
    }

    void SceneRenderer::EndScene()
    {
    }
}
