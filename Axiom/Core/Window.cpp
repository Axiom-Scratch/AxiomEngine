#include "Core/Window.h"

namespace Axiom::Platform
{
    Scope<Window> CreateWindow(const WindowProps& props);
}

namespace Axiom
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
        return Platform::CreateWindow(props);
    }
}
