#pragma once

#include "Renderer/SceneRenderer.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <QOpenGLWidget>
#include <QPointF>

#include <chrono>

class ViewportWidget final : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void UpdateCamera(float deltaTime);
    static void* GetProcAddress(const char* name);

    Axiom::Scene m_Scene;
    Axiom::SceneRenderer m_Renderer;
    Axiom::Entity m_CameraEntity;
    std::chrono::steady_clock::time_point m_LastFrameTime{};

    bool m_MoveForward = false;
    bool m_MoveBackward = false;
    bool m_MoveLeft = false;
    bool m_MoveRight = false;

    QPointF m_LastMousePos{};
    bool m_HasMousePos = false;
    QPointF m_MouseDelta{};
    bool m_Initialized = false;

    float m_MoveSpeed = 5.0f;
    float m_MouseSensitivity = 0.1f;
};
