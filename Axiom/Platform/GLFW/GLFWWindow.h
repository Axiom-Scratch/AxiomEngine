#pragma once

#include "Core/Window.h"

struct GLFWwindow;

namespace Axiom
{
    class GLFWWindow final : public Window
    {
    public:
        explicit GLFWWindow(const WindowProps& props);
        ~GLFWWindow() override;

        void OnUpdate() override;
        [[nodiscard]] uint32_t GetWidth() const override;
        [[nodiscard]] uint32_t GetHeight() const override;
        void SetEventCallback(const EventCallbackFn& callback) override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window = nullptr;

        struct WindowData
        {
            std::string Title;
            uint32_t Width = 0;
            uint32_t Height = 0;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
