#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/base_sink.h>

#include "Core.hpp"
#include "Console.hpp"

namespace spdlog::sinks
{
    /*  ====================================================================
        Custom Sink for the Client Logger

        The loggers in spdlog work using sinks. Sinks are objects that write
        logs to their targets (which can be a console, files, databases etc.)
        A logger can contain more than one sinks. Whenever a log is called on
        the logger, it iterates through each of the sinks and calls the
        sink/sink_it_ method on it. For more information on sinks, visit
        https://github.com/gabime/spdlog/wiki/4.-Sinks.

        Here we have created a ConsoleSink which writes to a Console object
        for use in the Editor. We inherit from base_sink which takes care of
        thread locking, making it suitable for multi-threading. The tempelated
        Mutex can be real or a dummy.
        ====================================================================
    */
    template <typename Mutex>
    class ConsoleSink : public base_sink<Mutex>
    {

    public:
        explicit ConsoleSink(QMBT::Ref<QMBT::Console> console)
        {
            m_Console = console;
        }

    protected:
        // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.raw contains pre formatted log
        void sink_it_(const spdlog::details::log_msg &msg) override
        {

            // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
            spdlog::memory_buf_t formatted;
            // We call the formatter that formats the msg.raw into a format passed in using spdlog::set_pattern
            base_sink<Mutex>::formatter_->format(msg, formatted);
            // Here, we write the now formatted log into our custom target, which is a custom console class in this case.
            // The msg.level is used for color information.
            m_Console->AddLog(fmt::to_string(formatted), msg.level);
        }

        void flush_() override
        {
            // TODO: Implement flush method in QMBT::Console class
        }

    private:
        QMBT::Ref<QMBT::Console> m_Console;
    };
}

namespace spdlog
{
    // A Factory for the multi threaded version of the CustomSink
    // Multi-threaded version is slower than a single-threaded version due to thread locking but is thread-safe
    template <typename Factory = synchronous_factory>
    inline std::shared_ptr<logger> ConsoleSink_mt(const std::string &logger_name, QMBT::Ref<QMBT::Console> console)
    {
        return Factory::template create<sinks::ConsoleSink<std::mutex>>(logger_name, console);
    }

}

namespace QMBT
{
    class Logger
    {
    public:
        static void InitializeCoreLogger();
        static void InitializeClientLogger(Ref<Console> console);

        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros for the different log levels
#define LOG_CORE_ERROR(...) ::QMBT::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::QMBT::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::QMBT::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_TRACE(...) ::QMBT::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::QMBT::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_ERROR(...) ::QMBT::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) ::QMBT::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) ::QMBT::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) ::QMBT::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CRITICAL(...) ::QMBT::Logger::GetClientLogger()->critical(__VA_ARGS__)
