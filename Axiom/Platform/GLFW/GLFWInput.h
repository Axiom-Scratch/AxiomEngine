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

    private:
        GLFWwindow* m_Window;
    };
}
