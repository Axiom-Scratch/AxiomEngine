#include "Core/Application.h"
#include "Core/Log.h"

#include "Event/EventDispatcher.h"

namespace Axiom
{
    Application::Application()
    {
        Log::Init();
        m_Window = Window::Create();
        m_Window->SetEventCallback([this](Event& event) { HandleEvent(event); });
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            OnUpdate();
            m_Window->OnUpdate();
        }
    }

    void Application::HandleEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent&) { return OnWindowClose(); });
        OnEvent(event);
    }

    bool Application::OnWindowClose()
    {
        m_Running = false;
        return true;
    }
}
