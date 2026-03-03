#include "Input/Input.h"

#include <exception>

namespace Axiom
{
    std::unique_ptr<PlatformInput> Input::s_PlatformInput;

    void Input::Initialize(std::unique_ptr<PlatformInput> input)
    {
        s_PlatformInput = std::move(input);
    }

    bool Input::IsKeyPressed(Key key)
    {
        return GetImpl().IsKeyPressedImpl(key);
    }

    bool Input::IsMousePressed(Mouse button)
    {
        return GetImpl().IsMousePressedImpl(button);
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        return GetImpl().GetMousePositionImpl();
    }

    PlatformInput& Input::GetImpl()
    {
        if (!s_PlatformInput)
        {
            std::terminate();
        }
        return *s_PlatformInput;
    }
}
