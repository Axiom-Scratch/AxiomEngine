#include "Input/Input.h"

#include <exception>

namespace Axiom
{
    std::unique_ptr<PlatformInput> Input::s_PlatformInput;
    std::pair<float, float> Input::s_MouseDeltaCache{ 0.0f, 0.0f };
    uint8_t Input::s_MouseDeltaAccessMask = 0;
    bool Input::s_MouseDeltaValid = false;

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

    std::pair<float, float> Input::GetMouseDelta()
    {
        return GetImpl().GetMouseDeltaImpl();
    }

    float Input::GetMouseDeltaX()
    {
        if (!s_MouseDeltaValid)
        {
            s_MouseDeltaCache = GetImpl().GetMouseDeltaImpl();
            s_MouseDeltaValid = true;
            s_MouseDeltaAccessMask = 0;
        }

        s_MouseDeltaAccessMask |= 0x1;
        const float value = s_MouseDeltaCache.first;
        if ((s_MouseDeltaAccessMask & 0x3) == 0x3)
        {
            s_MouseDeltaValid = false;
        }
        return value;
    }

    float Input::GetMouseDeltaY()
    {
        if (!s_MouseDeltaValid)
        {
            s_MouseDeltaCache = GetImpl().GetMouseDeltaImpl();
            s_MouseDeltaValid = true;
            s_MouseDeltaAccessMask = 0;
        }

        s_MouseDeltaAccessMask |= 0x2;
        const float value = s_MouseDeltaCache.second;
        if ((s_MouseDeltaAccessMask & 0x3) == 0x3)
        {
            s_MouseDeltaValid = false;
        }
        return value;
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
