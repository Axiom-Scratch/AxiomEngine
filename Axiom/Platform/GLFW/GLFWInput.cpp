#include "Platform/GLFW/GLFWInput.h"

#include <GLFW/glfw3.h>

namespace Axiom
{
    namespace
    {
        int ToGLFWKey(Key key)
        {
            switch (key)
            {
                case Key::A: return GLFW_KEY_A;
                case Key::B: return GLFW_KEY_B;
                case Key::C: return GLFW_KEY_C;
                case Key::D: return GLFW_KEY_D;
                case Key::E: return GLFW_KEY_E;
                case Key::F: return GLFW_KEY_F;
                case Key::G: return GLFW_KEY_G;
                case Key::H: return GLFW_KEY_H;
                case Key::I: return GLFW_KEY_I;
                case Key::J: return GLFW_KEY_J;
                case Key::K: return GLFW_KEY_K;
                case Key::L: return GLFW_KEY_L;
                case Key::M: return GLFW_KEY_M;
                case Key::N: return GLFW_KEY_N;
                case Key::O: return GLFW_KEY_O;
                case Key::P: return GLFW_KEY_P;
                case Key::Q: return GLFW_KEY_Q;
                case Key::R: return GLFW_KEY_R;
                case Key::S: return GLFW_KEY_S;
                case Key::T: return GLFW_KEY_T;
                case Key::U: return GLFW_KEY_U;
                case Key::V: return GLFW_KEY_V;
                case Key::W: return GLFW_KEY_W;
                case Key::X: return GLFW_KEY_X;
                case Key::Y: return GLFW_KEY_Y;
                case Key::Z: return GLFW_KEY_Z;
                case Key::D0: return GLFW_KEY_0;
                case Key::D1: return GLFW_KEY_1;
                case Key::D2: return GLFW_KEY_2;
                case Key::D3: return GLFW_KEY_3;
                case Key::D4: return GLFW_KEY_4;
                case Key::D5: return GLFW_KEY_5;
                case Key::D6: return GLFW_KEY_6;
                case Key::D7: return GLFW_KEY_7;
                case Key::D8: return GLFW_KEY_8;
                case Key::D9: return GLFW_KEY_9;
                case Key::Left: return GLFW_KEY_LEFT;
                case Key::Right: return GLFW_KEY_RIGHT;
                case Key::Up: return GLFW_KEY_UP;
                case Key::Down: return GLFW_KEY_DOWN;
                case Key::F1: return GLFW_KEY_F1;
                case Key::F2: return GLFW_KEY_F2;
                case Key::F3: return GLFW_KEY_F3;
                case Key::F4: return GLFW_KEY_F4;
                case Key::F5: return GLFW_KEY_F5;
                case Key::F6: return GLFW_KEY_F6;
                case Key::F7: return GLFW_KEY_F7;
                case Key::F8: return GLFW_KEY_F8;
                case Key::F9: return GLFW_KEY_F9;
                case Key::F10: return GLFW_KEY_F10;
                case Key::F11: return GLFW_KEY_F11;
                case Key::F12: return GLFW_KEY_F12;
                case Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
                case Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
                case Key::LeftAlt: return GLFW_KEY_LEFT_ALT;
                case Key::LeftSuper: return GLFW_KEY_LEFT_SUPER;
                case Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;
                case Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
                case Key::RightAlt: return GLFW_KEY_RIGHT_ALT;
                case Key::RightSuper: return GLFW_KEY_RIGHT_SUPER;
                case Key::Escape: return GLFW_KEY_ESCAPE;
                case Key::Enter: return GLFW_KEY_ENTER;
                case Key::Tab: return GLFW_KEY_TAB;
                case Key::Backspace: return GLFW_KEY_BACKSPACE;
                case Key::Space: return GLFW_KEY_SPACE;
            }
            return GLFW_KEY_UNKNOWN;
        }

        int ToGLFWMouse(Mouse button)
        {
            const auto value = static_cast<uint16_t>(button);
            switch (value)
            {
                case static_cast<uint16_t>(Mouse::Button0): return GLFW_MOUSE_BUTTON_1;
                case static_cast<uint16_t>(Mouse::Button1): return GLFW_MOUSE_BUTTON_2;
                case static_cast<uint16_t>(Mouse::Button2): return GLFW_MOUSE_BUTTON_3;
                case static_cast<uint16_t>(Mouse::Button3): return GLFW_MOUSE_BUTTON_4;
                case static_cast<uint16_t>(Mouse::Button4): return GLFW_MOUSE_BUTTON_5;
                case static_cast<uint16_t>(Mouse::Button5): return GLFW_MOUSE_BUTTON_6;
                case static_cast<uint16_t>(Mouse::Button6): return GLFW_MOUSE_BUTTON_7;
                case static_cast<uint16_t>(Mouse::Button7): return GLFW_MOUSE_BUTTON_8;
            }
            return GLFW_MOUSE_BUTTON_1;
        }
    }

    GLFWInput::GLFWInput(GLFWwindow* window)
        : m_Window(window)
    {
    }

    bool GLFWInput::IsKeyPressedImpl(Key key)
    {
        const int glfwKey = ToGLFWKey(key);
        if (glfwKey == GLFW_KEY_UNKNOWN)
        {
            return false;
        }
        const int state = glfwGetKey(m_Window, glfwKey);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GLFWInput::IsMousePressedImpl(Mouse button)
    {
        const int state = glfwGetMouseButton(m_Window, ToGLFWMouse(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> GLFWInput::GetMousePositionImpl()
    {
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(m_Window, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    std::pair<float, float> GLFWInput::GetMouseDeltaImpl()
    {
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(m_Window, &x, &y);

        if (!m_HasLast)
        {
            m_LastX = x;
            m_LastY = y;
            m_HasLast = true;
            return { 0.0f, 0.0f };
        }

        const float deltaX = static_cast<float>(x - m_LastX);
        const float deltaY = static_cast<float>(y - m_LastY);

        m_LastX = x;
        m_LastY = y;
        return { deltaX, deltaY };
    }
}
