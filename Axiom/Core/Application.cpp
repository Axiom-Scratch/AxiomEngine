#include "Core/Application.h"
#include "Core/Log.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include "Event/EventDispatcher.h"

namespace Axiom
{
    Application::Application()
    {
        Log::Init();
        m_Window = Window::Create();
        m_Window->SetEventCallback([this](Event& event) { HandleEvent(event); });
        Renderer::Init();
        RenderCommand::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
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
