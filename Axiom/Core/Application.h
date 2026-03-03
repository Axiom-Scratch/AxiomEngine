#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Event/Event.h"

namespace Axiom
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    protected:
        virtual void OnUpdate() {}
        virtual void OnEvent(Event&) {}
        Window& GetWindow();
        const Window& GetWindow() const;

    private:
        void HandleEvent(Event& event);
        bool OnWindowClose();

        Scope<Window> m_Window;
        bool m_Running = true;
    };

    Scope<Application> CreateApplication();
}
