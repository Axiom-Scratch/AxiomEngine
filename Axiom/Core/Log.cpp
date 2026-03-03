#include "Core/Log.h"

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <mutex>
#include <memory>

namespace Axiom
{
    namespace
    {
        std::once_flag& InitFlag()
        {
            static std::once_flag flag;
            return flag;
        }

        std::shared_ptr<spdlog::logger>& CoreLogger()
        {
            static std::shared_ptr<spdlog::logger> logger;
            return logger;
        }

        std::shared_ptr<spdlog::logger>& ClientLogger()
        {
            static std::shared_ptr<spdlog::logger> logger;
            return logger;
        }

        spdlog::level::level_enum DefaultLevel()
        {
#if defined(NDEBUG)
            return spdlog::level::info;
#else
            return spdlog::level::trace;
#endif
        }

        void ApplyLevel(const std::shared_ptr<spdlog::logger>& logger, spdlog::level::level_enum level)
        {
            if (logger)
            {
                logger->set_level(level);
            }
        }
    }

    void Log::Init()
    {
        std::call_once(InitFlag(), []
        {
            auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            sink->set_pattern("[%^%T] [%n] [%l]: %v%$");

            CoreLogger() = std::make_shared<spdlog::logger>("Core", sink);
            ClientLogger() = std::make_shared<spdlog::logger>("Client", sink);

            const auto level = DefaultLevel();
            ApplyLevel(CoreLogger(), level);
            ApplyLevel(ClientLogger(), level);
        });
    }

    void Log::CoreLog(Level level, std::string_view message)
    {
        Init();
        auto& logger = CoreLogger();
        if (!logger)
        {
            return;
        }
        switch (level)
        {
            case Level::Trace: logger->trace(message); break;
            case Level::Info: logger->info(message); break;
            case Level::Warn: logger->warn(message); break;
            case Level::Error: logger->error(message); break;
            case Level::Critical: logger->critical(message); break;
        }
    }

    void Log::ClientLog(Level level, std::string_view message)
    {
        Init();
        auto& logger = ClientLogger();
        if (!logger)
        {
            return;
        }
        switch (level)
        {
            case Level::Trace: logger->trace(message); break;
            case Level::Info: logger->info(message); break;
            case Level::Warn: logger->warn(message); break;
            case Level::Error: logger->error(message); break;
            case Level::Critical: logger->critical(message); break;
        }
    }
}
