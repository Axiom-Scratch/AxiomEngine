#include "Axiom.h"

#include <cstdio>

class SandboxApp final : public Axiom::Application
{
protected:
    void OnUpdate() override
    {
        const bool pressed = Axiom::Input::IsKeyPressed(Axiom::Key::W);
        std::printf("W is %s\n", pressed ? "pressed" : "released");
        const auto [mx, my] = Axiom::Input::GetMousePosition();
        std::printf("Mouse Position: (%.1f, %.1f)\n", mx, my);
    }
};

Axiom::Scope<Axiom::Application> Axiom::CreateApplication()
{
    return Axiom::CreateScope<SandboxApp>();
}

#include "EntryPoint.h"
