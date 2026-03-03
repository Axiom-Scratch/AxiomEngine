#include "Platform/GLFW/GLFWCursor.h"

#include <GLFW/glfw3.h>

namespace Axiom
{
    void ToggleCursor(GLFWwindow* window)
    {
        if (!window)
        {
            return;
        }

        const int mode = glfwGetInputMode(window, GLFW_CURSOR);
        const int nextMode = (mode == GLFW_CURSOR_DISABLED) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(window, GLFW_CURSOR, nextMode);
    }
}
