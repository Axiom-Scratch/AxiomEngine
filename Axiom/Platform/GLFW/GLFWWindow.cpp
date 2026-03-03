#include "Platform/GLFW/GLFWWindow.h"

#include "Core/Base.h"
#include "Event/Event.h"
#include "Input/Input.h"
#include "Platform/GLFW/GLFWInput.h"
#include "Renderer/RenderCommand.h"

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <memory>
#include <optional>

namespace Axiom
{
    namespace
    {
        std::optional<Key> FromGLFWKey(int key)
        {
            switch (key)
            {
                case GLFW_KEY_A: return Key::A;
                case GLFW_KEY_B: return Key::B;
                case GLFW_KEY_C: return Key::C;
                case GLFW_KEY_D: return Key::D;
                case GLFW_KEY_E: return Key::E;
                case GLFW_KEY_F: return Key::F;
                case GLFW_KEY_G: return Key::G;
                case GLFW_KEY_H: return Key::H;
                case GLFW_KEY_I: return Key::I;
                case GLFW_KEY_J: return Key::J;
                case GLFW_KEY_K: return Key::K;
                case GLFW_KEY_L: return Key::L;
                case GLFW_KEY_M: return Key::M;
                case GLFW_KEY_N: return Key::N;
                case GLFW_KEY_O: return Key::O;
                case GLFW_KEY_P: return Key::P;
                case GLFW_KEY_Q: return Key::Q;
                case GLFW_KEY_R: return Key::R;
                case GLFW_KEY_S: return Key::S;
                case GLFW_KEY_T: return Key::T;
                case GLFW_KEY_U: return Key::U;
                case GLFW_KEY_V: return Key::V;
                case GLFW_KEY_W: return Key::W;
                case GLFW_KEY_X: return Key::X;
                case GLFW_KEY_Y: return Key::Y;
                case GLFW_KEY_Z: return Key::Z;
                case GLFW_KEY_0: return Key::D0;
                case GLFW_KEY_1: return Key::D1;
                case GLFW_KEY_2: return Key::D2;
                case GLFW_KEY_3: return Key::D3;
                case GLFW_KEY_4: return Key::D4;
                case GLFW_KEY_5: return Key::D5;
                case GLFW_KEY_6: return Key::D6;
                case GLFW_KEY_7: return Key::D7;
                case GLFW_KEY_8: return Key::D8;
                case GLFW_KEY_9: return Key::D9;
                case GLFW_KEY_LEFT: return Key::Left;
                case GLFW_KEY_RIGHT: return Key::Right;
                case GLFW_KEY_UP: return Key::Up;
                case GLFW_KEY_DOWN: return Key::Down;
                case GLFW_KEY_F1: return Key::F1;
                case GLFW_KEY_F2: return Key::F2;
                case GLFW_KEY_F3: return Key::F3;
                case GLFW_KEY_F4: return Key::F4;
                case GLFW_KEY_F5: return Key::F5;
                case GLFW_KEY_F6: return Key::F6;
                case GLFW_KEY_F7: return Key::F7;
                case GLFW_KEY_F8: return Key::F8;
                case GLFW_KEY_F9: return Key::F9;
                case GLFW_KEY_F10: return Key::F10;
                case GLFW_KEY_F11: return Key::F11;
                case GLFW_KEY_F12: return Key::F12;
                case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
                case GLFW_KEY_LEFT_CONTROL: return Key::LeftControl;
                case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
                case GLFW_KEY_LEFT_SUPER: return Key::LeftSuper;
                case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
                case GLFW_KEY_RIGHT_CONTROL: return Key::RightControl;
                case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
                case GLFW_KEY_RIGHT_SUPER: return Key::RightSuper;
                case GLFW_KEY_ESCAPE: return Key::Escape;
                case GLFW_KEY_ENTER: return Key::Enter;
                case GLFW_KEY_TAB: return Key::Tab;
                case GLFW_KEY_BACKSPACE: return Key::Backspace;
                case GLFW_KEY_SPACE: return Key::Space;
            }
            return std::nullopt;
        }

        std::optional<Mouse> FromGLFWMouse(int button)
        {
            switch (button)
            {
                case GLFW_MOUSE_BUTTON_1: return Mouse::Button0;
                case GLFW_MOUSE_BUTTON_2: return Mouse::Button1;
                case GLFW_MOUSE_BUTTON_3: return Mouse::Button2;
                case GLFW_MOUSE_BUTTON_4: return Mouse::Button3;
                case GLFW_MOUSE_BUTTON_5: return Mouse::Button4;
                case GLFW_MOUSE_BUTTON_6: return Mouse::Button5;
                case GLFW_MOUSE_BUTTON_7: return Mouse::Button6;
                case GLFW_MOUSE_BUTTON_8: return Mouse::Button7;
            }
            return std::nullopt;
        }
    }

    GLFWWindow::GLFWWindow(const WindowProps& props)
    {
        Init(props);
    }

    GLFWWindow::~GLFWWindow()
    {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps& props)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            std::abort();
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.EventCallback = [](Event&) {};

        m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            std::abort();
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        Input::Initialize(std::make_unique<GLFWInput>(m_Window));

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int, int action, int)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            const auto mapped = FromGLFWKey(key);
            if (!mapped)
            {
                return;
            }

            if (action == GLFW_PRESS)
            {
                KeyPressedEvent event(*mapped, false);
                data.EventCallback(event);
                return;
            }

            if (action == GLFW_REPEAT)
            {
                KeyPressedEvent event(*mapped, true);
                data.EventCallback(event);
                return;
            }

            if (action == GLFW_RELEASE)
            {
                KeyReleasedEvent event(*mapped);
                data.EventCallback(event);
            }
        });

        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Width = static_cast<uint32_t>(width);
            data.Height = static_cast<uint32_t>(height);
            RenderCommand::SetViewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            const auto mapped = FromGLFWMouse(button);
            if (!mapped)
            {
                return;
            }

            if (action == GLFW_PRESS)
            {
                MouseButtonPressedEvent event(*mapped);
                data.EventCallback(event);
                return;
            }

            if (action == GLFW_RELEASE)
            {
                MouseButtonReleasedEvent event(*mapped);
                data.EventCallback(event);
            }
        });
    }

    void GLFWWindow::Shutdown()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

    void GLFWWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    uint32_t GLFWWindow::GetWidth() const
    {
        return m_Data.Width;
    }

    uint32_t GLFWWindow::GetHeight() const
    {
        return m_Data.Height;
    }

    void* GLFWWindow::GetNativeHandle() const
    {
        return m_Window;
    }

    void GLFWWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Data.EventCallback = callback;
    }
}

namespace Axiom::Platform
{
    Scope<Window> CreateWindow(const WindowProps& props)
    {
        return CreateScope<GLFWWindow>(props);
    }
}
