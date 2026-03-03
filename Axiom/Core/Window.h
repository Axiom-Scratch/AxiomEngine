#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "Core/Base.h"
#include "Event/Event.h"

namespace Axiom
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        explicit WindowProps(std::string title = "Axiom", uint32_t width = 1280, uint32_t height = 720)
            : Title(std::move(title)), Width(width), Height(height)
        {
        }
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;
        virtual void OnUpdate() = 0;
        [[nodiscard]] virtual uint32_t GetWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;
        [[nodiscard]] virtual void* GetNativeHandle() const = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}
