#include "Axiom.h"

#include <glm/glm.hpp>

class SandboxApp final : public Axiom::Application
{
public:
    SandboxApp()
    {
        Axiom::Renderer::Init();

        const float vertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        };

        const uint32_t indices[] = { 0, 1, 2 };

        m_VertexArray = Axiom::VertexArray::Create();

        auto vertexBuffer = Axiom::VertexBuffer::Create(vertices, sizeof(vertices));
        const Axiom::BufferLayout layout = {
            { Axiom::ShaderDataType::Float3, "a_Position" }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        auto indexBuffer = Axiom::IndexBuffer::Create(indices, 3);
        m_VertexArray->SetIndexBuffer(indexBuffer);

        const std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}
)";

        const std::string fragmentSource = R"(
#version 330 core
out vec4 color;

void main()
{
    color = vec4(0.2, 0.7, 0.3, 1.0);
}
)";

        m_Shader = Axiom::Shader::Create(vertexSource, fragmentSource);
    }

protected:
    void OnUpdate() override
    {
        const float aspect = 1280.0f / 720.0f;
        Axiom::Camera camera(45.0f, aspect, 0.1f, 100.0f);

        Axiom::Renderer::BeginScene(camera);
        Axiom::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
        Axiom::Renderer::EndScene();
    }

private:
    std::shared_ptr<Axiom::Shader> m_Shader;
    std::shared_ptr<Axiom::VertexArray> m_VertexArray;
};

Axiom::Scope<Axiom::Application> Axiom::CreateApplication()
{
    return Axiom::CreateScope<SandboxApp>();
}

#include "EntryPoint.h"
