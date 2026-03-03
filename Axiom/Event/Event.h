#pragma once

#include "Input/Input.h"

#include <cstdint>

namespace Axiom
{
    enum class EventType : uint8_t
    {
        WindowClose,
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased
    };

    class Event
    {
    public:
        virtual ~Event() = default;
        bool Handled = false;
        virtual EventType GetType() const = 0;
    };

    class WindowCloseEvent final : public Event
    {
    public:
        static EventType GetStaticType()
        {
            return EventType::WindowClose;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }
    };

    class KeyPressedEvent final : public Event
    {
    public:
        KeyPressedEvent(Key key, bool repeat)
            : m_Key(key), m_Repeat(repeat)
        {
        }

        [[nodiscard]] Key GetKey() const
        {
            return m_Key;
        }

        [[nodiscard]] bool IsRepeat() const
        {
            return m_Repeat;
        }

        static EventType GetStaticType()
        {
            return EventType::KeyPressed;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }

    private:
        Key m_Key;
        bool m_Repeat;
    };

    class KeyReleasedEvent final : public Event
    {
    public:
        explicit KeyReleasedEvent(Key key)
            : m_Key(key)
        {
        }

        [[nodiscard]] Key GetKey() const
        {
            return m_Key;
        }

        static EventType GetStaticType()
        {
            return EventType::KeyReleased;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }

    private:
        Key m_Key;
    };

    class MouseMovedEvent final : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_X(x), m_Y(y)
        {
        }

        [[nodiscard]] float GetX() const
        {
            return m_X;
        }

        [[nodiscard]] float GetY() const
        {
            return m_Y;
        }

        static EventType GetStaticType()
        {
            return EventType::MouseMoved;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }

    private:
        float m_X;
        float m_Y;
    };

    class MouseButtonPressedEvent final : public Event
    {
    public:
        explicit MouseButtonPressedEvent(Mouse button)
            : m_Button(button)
        {
        }

        [[nodiscard]] Mouse GetButton() const
        {
            return m_Button;
        }

        static EventType GetStaticType()
        {
            return EventType::MouseButtonPressed;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }

    private:
        Mouse m_Button;
    };

    class MouseButtonReleasedEvent final : public Event
    {
    public:
        explicit MouseButtonReleasedEvent(Mouse button)
            : m_Button(button)
        {
        }

        [[nodiscard]] Mouse GetButton() const
        {
            return m_Button;
        }

        static EventType GetStaticType()
        {
            return EventType::MouseButtonReleased;
        }

        EventType GetType() const override
        {
            return GetStaticType();
        }

    private:
        Mouse m_Button;
    };
}
