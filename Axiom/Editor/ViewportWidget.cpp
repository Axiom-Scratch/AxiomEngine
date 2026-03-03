#include "Editor/ViewportWidget.h"

#include "Renderer/Mesh.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include <QOpenGLContext>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtGui/qopengl.h>
#include <QByteArray>

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

namespace
{
    using GLADloadfunc = void* (*)(const char*);

    extern "C" int gladLoadGL(GLADloadfunc load);

    int gladLoadGLLoader(GLADloadfunc load)
    {
        return gladLoadGL(load);
    }
}

namespace Axiom::Editor
{
    ViewportWidget::ViewportWidget(QWidget* parent)
        : QOpenGLWidget(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
    }

    void* ViewportWidget::GetProcAddress(const char* name)
    {
        auto* context = QOpenGLContext::currentContext();
        if (!context)
        {
            return nullptr;
        }

        return reinterpret_cast<void*>(context->getProcAddress(QByteArray(name)));
    }

    void ViewportWidget::initializeGL()
    {
        RenderCommand::SetGLLoader(&ViewportWidget::GetProcAddress);

        gladLoadGLLoader(&ViewportWidget::GetProcAddress);
        glEnable(GL_DEPTH_TEST);
        Renderer::Init();

        m_Scene = std::make_unique<Scene>();

        auto cubeMesh = Mesh::CreateCube();

        auto cube1 = m_Scene->CreateEntity("Cube 1");
        cube1.AddComponent<MeshComponent>(cubeMesh);
        cube1.GetComponent<TransformComponent>().Translation = Math::Vec3(-1.5f, 0.0f, 0.0f);

        auto cube2 = m_Scene->CreateEntity("Cube 2");
        cube2.AddComponent<MeshComponent>(cubeMesh);
        cube2.GetComponent<TransformComponent>().Translation = Math::Vec3(1.5f, 0.0f, 0.0f);

        m_LastFrameTime = std::chrono::steady_clock::now();
    }

    void ViewportWidget::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);
        m_AspectRatio = (h > 0) ? static_cast<float>(w) / static_cast<float>(h) : 1.0f;
    }

    void ViewportWidget::paintGL()
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> delta = now - m_LastFrameTime;
        m_LastFrameTime = now;

        const float deltaTime = delta.count();

        UpdateCamera(deltaTime);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const Math::Mat4 projection = glm::perspective(
            glm::radians(45.0f),
            m_AspectRatio,
            0.1f,
            100.0f);
        const Math::Mat4 view = m_Camera.GetViewMatrix();
        const Math::Mat4 viewProjection = projection * view;

        if (m_Scene)
        {
            m_Scene->OnUpdate(deltaTime);
            m_Scene->OnRender(m_Renderer, viewProjection);
        }

        update();
    }

    void ViewportWidget::keyPressEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            return;
        }

        switch (event->key())
        {
        case Qt::Key_W:
            m_MoveForward = true;
            break;
        case Qt::Key_S:
            m_MoveBackward = true;
            break;
        case Qt::Key_A:
            m_MoveLeft = true;
            break;
        case Qt::Key_D:
            m_MoveRight = true;
            break;
        default:
            break;
        }
    }

    void ViewportWidget::keyReleaseEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            return;
        }

        switch (event->key())
        {
        case Qt::Key_W:
            m_MoveForward = false;
            break;
        case Qt::Key_S:
            m_MoveBackward = false;
            break;
        case Qt::Key_A:
            m_MoveLeft = false;
            break;
        case Qt::Key_D:
            m_MoveRight = false;
            break;
        default:
            break;
        }
    }

    void ViewportWidget::mouseMoveEvent(QMouseEvent* event)
    {
        const float xpos = static_cast<float>(event->position().x());
        const float ypos = static_cast<float>(event->position().y());

        if (m_FirstMouse)
        {
            m_LastMouseX = xpos;
            m_LastMouseY = ypos;
            m_FirstMouse = false;
            return;
        }

        float xoffset = xpos - m_LastMouseX;
        float yoffset = m_LastMouseY - ypos;
        m_LastMouseX = xpos;
        m_LastMouseY = ypos;

        xoffset *= m_Camera.Sensitivity;
        yoffset *= m_Camera.Sensitivity;

        m_Camera.Yaw += xoffset;
        m_Camera.Pitch = std::clamp(m_Camera.Pitch + yoffset, -89.0f, 89.0f);
    }

    Math::Vec3 ViewportWidget::EditorCamera::GetForward() const
    {
        const float yawRadians = glm::radians(Yaw);
        const float pitchRadians = glm::radians(Pitch);

        Math::Vec3 direction;
        direction.x = std::cos(yawRadians) * std::cos(pitchRadians);
        direction.y = std::sin(pitchRadians);
        direction.z = std::sin(yawRadians) * std::cos(pitchRadians);
        return glm::normalize(direction);
    }

    Math::Mat4 ViewportWidget::EditorCamera::GetViewMatrix() const
    {
        const Math::Vec3 forward = GetForward();
        return glm::lookAt(Position, Position + forward, Math::Vec3(0.0f, 1.0f, 0.0f));
    }

    void ViewportWidget::UpdateCamera(float deltaTime)
    {
        const Math::Vec3 forward = m_Camera.GetForward();
        const Math::Vec3 right = glm::normalize(glm::cross(forward, Math::Vec3(0.0f, 1.0f, 0.0f)));
        const float velocity = m_Camera.Speed * deltaTime;

        if (m_MoveForward)
        {
            m_Camera.Position += forward * velocity;
        }
        if (m_MoveBackward)
        {
            m_Camera.Position -= forward * velocity;
        }
        if (m_MoveLeft)
        {
            m_Camera.Position -= right * velocity;
        }
        if (m_MoveRight)
        {
            m_Camera.Position += right * velocity;
        }
    }
}
