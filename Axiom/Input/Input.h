#pragma once

#include <cstdint>
#include <memory>
#include <utility>

namespace Axiom
{
    enum class Key : uint16_t
    {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
        Left,
        Right,
        Up,
        Down,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Escape,
        Enter,
        Tab,
        Backspace,
        Space
    };

    enum class Mouse : uint16_t
    {
        Button0 = 0,
        Button1,
        Button2,
        Button3,
        Button4,
        Button5,
        Button6,
        Button7,
        Left = Button0,
        Right = Button1,
        Middle = Button2
    };

    class PlatformInput
    {
    public:
        virtual ~PlatformInput() = default;
        virtual bool IsKeyPressedImpl(Key key) = 0;
        virtual bool IsMousePressedImpl(Mouse button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
    };

    class Input
    {
    public:
        static void Initialize(std::unique_ptr<PlatformInput> input);
        static bool IsKeyPressed(Key key);
        static bool IsMousePressed(Mouse button);
        static std::pair<float, float> GetMousePosition();

    private:
        static PlatformInput& GetImpl();
        static std::unique_ptr<PlatformInput> s_PlatformInput;
    };
}
