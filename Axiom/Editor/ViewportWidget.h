#pragma once

#include "Renderer/SceneRenderer.h"
#include "Scene/Scene.h"
#include "Math/Math.h"

#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QMouseEvent>

#include <chrono>
#include <memory>

namespace Axiom::Editor
{
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
        struct EditorCamera
        {
            Math::Vec3 Position{ 0.0f, 0.0f, 5.0f };
            float Yaw = -90.0f;
            float Pitch = 0.0f;
            float Speed = 5.0f;
            float Sensitivity = 0.1f;

            [[nodiscard]] Math::Vec3 GetForward() const;
            [[nodiscard]] Math::Mat4 GetViewMatrix() const;
        };

        static void* GetProcAddress(const char* name);
        void UpdateCamera(float deltaTime);

        std::unique_ptr<Scene> m_Scene;
        SceneRenderer m_Renderer;
        EditorCamera m_Camera{};
        float m_AspectRatio = 1.0f;
        bool m_MoveForward = false;
        bool m_MoveBackward = false;
        bool m_MoveLeft = false;
        bool m_MoveRight = false;
        bool m_FirstMouse = true;
        float m_LastMouseX = 0.0f;
        float m_LastMouseY = 0.0f;
        std::chrono::steady_clock::time_point m_LastFrameTime{};
    };
}
