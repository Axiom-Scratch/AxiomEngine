#pragma once

#include "Event/Event.h"

#include <utility>

namespace Axiom
{
    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event)
            : m_Event(event)
        {
        }

        template<typename T, typename F>
        bool Dispatch(F&& func)
        {
            if (m_Event.GetType() == T::GetStaticType())
            {
                m_Event.Handled = std::forward<F>(func)(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
}
