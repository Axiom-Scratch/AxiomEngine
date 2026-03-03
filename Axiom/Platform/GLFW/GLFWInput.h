#pragma once

#include "Input/Input.h"

struct GLFWwindow;

namespace Axiom
{
    class GLFWInput final : public PlatformInput
    {
    public:
        explicit GLFWInput(GLFWwindow* window);

        bool IsKeyPressedImpl(Key key) override;
        bool IsMousePressedImpl(Mouse button) override;
        std::pair<float, float> GetMousePositionImpl() override;
        std::pair<float, float> GetMouseDeltaImpl() override;

    private:
        GLFWwindow* m_Window;
        double m_LastX = 0.0;
        double m_LastY = 0.0;
        bool m_HasLast = false;
    };
}
