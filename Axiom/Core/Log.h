#pragma once

#include <cstdint>
#include <format>
#include <string_view>
#include <utility>

namespace Axiom
{
    class Log
    {
    public:
        static void Init();

        template<typename... Args>
        static void CoreTrace(std::format_string<Args...> format, Args&&... args)
        {
            CoreLog(Level::Trace, std::format(format, std::forward<Args>(args)...));
        }

        static void CoreTrace(std::string_view message)
        {
            CoreLog(Level::Trace, message);
        }

        template<typename... Args>
        static void CoreInfo(std::format_string<Args...> format, Args&&... args)
        {
            CoreLog(Level::Info, std::format(format, std::forward<Args>(args)...));
        }

        static void CoreInfo(std::string_view message)
        {
            CoreLog(Level::Info, message);
        }

        template<typename... Args>
        static void CoreWarn(std::format_string<Args...> format, Args&&... args)
        {
            CoreLog(Level::Warn, std::format(format, std::forward<Args>(args)...));
        }

        static void CoreWarn(std::string_view message)
        {
            CoreLog(Level::Warn, message);
        }

        template<typename... Args>
        static void CoreError(std::format_string<Args...> format, Args&&... args)
        {
            CoreLog(Level::Error, std::format(format, std::forward<Args>(args)...));
        }

        static void CoreError(std::string_view message)
        {
            CoreLog(Level::Error, message);
        }

        template<typename... Args>
        static void CoreCritical(std::format_string<Args...> format, Args&&... args)
        {
            CoreLog(Level::Critical, std::format(format, std::forward<Args>(args)...));
        }

        static void CoreCritical(std::string_view message)
        {
            CoreLog(Level::Critical, message);
        }

        template<typename... Args>
        static void Trace(std::format_string<Args...> format, Args&&... args)
        {
            ClientLog(Level::Trace, std::format(format, std::forward<Args>(args)...));
        }

        static void Trace(std::string_view message)
        {
            ClientLog(Level::Trace, message);
        }

        template<typename... Args>
        static void Info(std::format_string<Args...> format, Args&&... args)
        {
            ClientLog(Level::Info, std::format(format, std::forward<Args>(args)...));
        }

        static void Info(std::string_view message)
        {
            ClientLog(Level::Info, message);
        }

        template<typename... Args>
        static void Warn(std::format_string<Args...> format, Args&&... args)
        {
            ClientLog(Level::Warn, std::format(format, std::forward<Args>(args)...));
        }

        static void Warn(std::string_view message)
        {
            ClientLog(Level::Warn, message);
        }

        template<typename... Args>
        static void Error(std::format_string<Args...> format, Args&&... args)
        {
            ClientLog(Level::Error, std::format(format, std::forward<Args>(args)...));
        }

        static void Error(std::string_view message)
        {
            ClientLog(Level::Error, message);
        }

        template<typename... Args>
        static void Critical(std::format_string<Args...> format, Args&&... args)
        {
            ClientLog(Level::Critical, std::format(format, std::forward<Args>(args)...));
        }

        static void Critical(std::string_view message)
        {
            ClientLog(Level::Critical, message);
        }

    private:
        enum class Level : uint8_t
        {
            Trace,
            Info,
            Warn,
            Error,
            Critical
        };

        static void CoreLog(Level level, std::string_view message);
        static void ClientLog(Level level, std::string_view message);
    };
}

#define AXIOM_CORE_TRACE(...) ::Axiom::Log::CoreTrace(__VA_ARGS__)
#define AXIOM_CORE_INFO(...) ::Axiom::Log::CoreInfo(__VA_ARGS__)
#define AXIOM_CORE_WARN(...) ::Axiom::Log::CoreWarn(__VA_ARGS__)
#define AXIOM_CORE_ERROR(...) ::Axiom::Log::CoreError(__VA_ARGS__)
#define AXIOM_CORE_CRITICAL(...) ::Axiom::Log::CoreCritical(__VA_ARGS__)

#define AXIOM_TRACE(...) ::Axiom::Log::Trace(__VA_ARGS__)
#define AXIOM_INFO(...) ::Axiom::Log::Info(__VA_ARGS__)
#define AXIOM_WARN(...) ::Axiom::Log::Warn(__VA_ARGS__)
#define AXIOM_ERROR(...) ::Axiom::Log::Error(__VA_ARGS__)
#define AXIOM_CRITICAL(...) ::Axiom::Log::Critical(__VA_ARGS__)
