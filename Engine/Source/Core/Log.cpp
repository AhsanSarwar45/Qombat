#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace QMBT
{
    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

    void Logger::InitializeCoreLogger()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("QMBT");
        s_CoreLogger->set_level(spdlog::level::trace);
    }

    void Logger::InitializeClientLogger(Ref<Console> console)
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        s_ClientLogger = spdlog::ConsoleSink_mt("App", console);
        s_ClientLogger->set_level(spdlog::level::trace);
    }

    // Note: To add multiple sinks, see https://github.com/gabime/spdlog/wiki/2.-Creating-loggers under section "Creating loggers with multiple sinks"
}