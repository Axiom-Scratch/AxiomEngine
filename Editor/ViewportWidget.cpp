#include "ViewportWidget.h"

#include "Core/Log.h"
#include "Math/Math.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Scene/Components.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLContext>

#include <algorithm>
#include <cmath>

namespace
{
    using GLADloadfunc = void* (*)(const char*);

    extern "C" int gladLoadGL(GLADloadfunc load);

    int gladLoadGLLoader(GLADloadfunc load)
    {
        return gladLoadGL(load);
    }

    Axiom::Math::Vec3 GetForward(const Axiom::Math::Vec3& rotation)
    {
        const float yawRadians = glm::radians(rotation.y);
        const float pitchRadians = glm::radians(rotation.x);

        Axiom::Math::Vec3 direction;
        direction.x = std::cos(yawRadians) * std::cos(pitchRadians);
        direction.y = std::sin(pitchRadians);
        direction.z = std::sin(yawRadians) * std::cos(pitchRadians);
        return glm::normalize(direction);
    }
}

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
    Axiom::RenderCommand::SetGLLoader(&ViewportWidget::GetProcAddress);

    const int gladStatus = gladLoadGLLoader(&ViewportWidget::GetProcAddress);
    if (gladStatus == 0)
    {
        AXIOM_CORE_ERROR("Failed to initialize GLAD for Qt viewport.");
        return;
    }

    Axiom::Renderer::Init();
    glEnable(GL_DEPTH_TEST);

    m_LastFrameTime = std::chrono::steady_clock::now();
    m_Initialized = true;

    m_CameraEntity = m_Scene.CreateEntity("EditorCamera");
    m_CameraEntity.AddComponent<Axiom::CameraComponent>();
    auto& cameraTransform = m_CameraEntity.GetComponent<Axiom::TransformComponent>();
    cameraTransform.Translation = Axiom::Math::Vec3(0.0f, 0.0f, 5.0f);
    cameraTransform.Rotation = Axiom::Math::Vec3(0.0f, -90.0f, 0.0f);

    const auto cubeMesh = Axiom::Mesh::CreateCube();
    if (cubeMesh)
    {
        auto cubeOne = m_Scene.CreateEntity("CubeOne");
        cubeOne.AddComponent<Axiom::MeshComponent>(cubeMesh);
        cubeOne.GetComponent<Axiom::TransformComponent>().Translation = Axiom::Math::Vec3(-1.5f, 0.0f, 0.0f);

        auto cubeTwo = m_Scene.CreateEntity("CubeTwo");
        cubeTwo.AddComponent<Axiom::MeshComponent>(cubeMesh);
        cubeTwo.GetComponent<Axiom::TransformComponent>().Translation = Axiom::Math::Vec3(1.5f, 0.0f, 0.0f);
    }
}

void ViewportWidget::resizeGL(int w, int h)
{
    Axiom::RenderCommand::SetViewport(0, 0, static_cast<uint32_t>(w), static_cast<uint32_t>(h));

    if (h <= 0 || !m_CameraEntity)
    {
        return;
    }

    const float aspect = static_cast<float>(w) / static_cast<float>(h);
    auto& cameraComponent = m_CameraEntity.GetComponent<Axiom::CameraComponent>();
    cameraComponent.CameraData = Axiom::Camera(45.0f, aspect, 0.1f, 100.0f);
}

void ViewportWidget::paintGL()
{
    if (!m_Initialized)
    {
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<float> delta = now - m_LastFrameTime;
    m_LastFrameTime = now;

    const float deltaTime = delta.count();
    UpdateCamera(deltaTime);

    m_Scene.OnUpdate(deltaTime);
    m_Scene.OnRender(m_Renderer);

    update();
}

void ViewportWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
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

    event->accept();
}

void ViewportWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
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

    event->accept();
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
    const QPointF position = event->position();
    if (!m_HasMousePos)
    {
        m_LastMousePos = position;
        m_HasMousePos = true;
        event->accept();
        return;
    }

    const QPointF delta = position - m_LastMousePos;
    m_LastMousePos = position;
    m_MouseDelta += delta;
    event->accept();
}

void ViewportWidget::UpdateCamera(float deltaTime)
{
    if (!m_CameraEntity)
    {
        return;
    }

    auto& cameraTransform = m_CameraEntity.GetComponent<Axiom::TransformComponent>();

    const float speed = m_MoveSpeed * deltaTime;
    const auto forward = GetForward(cameraTransform.Rotation);
    const auto right = glm::normalize(glm::cross(forward, Axiom::Math::Vec3(0.0f, 1.0f, 0.0f)));

    if (m_MoveForward)
    {
        cameraTransform.Translation += forward * speed;
    }
    if (m_MoveBackward)
    {
        cameraTransform.Translation -= forward * speed;
    }
    if (m_MoveLeft)
    {
        cameraTransform.Translation -= right * speed;
    }
    if (m_MoveRight)
    {
        cameraTransform.Translation += right * speed;
    }

    const QPointF delta = m_MouseDelta;
    m_MouseDelta = QPointF(0.0, 0.0);

    cameraTransform.Rotation.y += static_cast<float>(delta.x()) * m_MouseSensitivity;
    cameraTransform.Rotation.x = std::clamp(cameraTransform.Rotation.x - static_cast<float>(delta.y()) * m_MouseSensitivity,
        -89.0f, 89.0f);
}
