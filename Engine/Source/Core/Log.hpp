#pragma once

#include <mutex>
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/base_sink.h>

#include "Core.hpp"
#include "Console.hpp"

namespace spdlog::sinks
{
    //Custom Sink for the Client Logger
    template <typename Mutex>
    class ConsoleSink : public spdlog::sinks::base_sink<Mutex>
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
            base_sink<Mutex>::formatter_->format(msg, formatted);
            m_Console->AddLog(fmt::to_string(formatted), msg.level);
        }

        void flush_() override
        {
            std::cout << std::flush;
        }

    private:
        QMBT::Ref<QMBT::Console> m_Console;
    };
}

namespace spdlog
{
    // A FActory for the multi threaded version of the CustomSink
    template <typename Factory = spdlog::synchronous_factory>
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

// Core Log macros for the different log levels
#define QMBT_CORE_ERROR(...) ::QMBT::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define QMBT_CORE_WARN(...) ::QMBT::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define QMBT_CORE_INFO(...) ::QMBT::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define QMBT_CORE_TRACE(...) ::QMBT::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define QMBT_CORE_CRITICAL(...) ::QMBT::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define QMBT_ERROR(...) ::QMBT::Logger::GetClientLogger()->error(__VA_ARGS__)
#define QMBT_WARN(...) ::QMBT::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define QMBT_INFO(...) ::QMBT::Logger::GetClientLogger()->info(__VA_ARGS__)
#define QMBT_TRACE(...) ::QMBT::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define QMBT_CRITICAL(...) ::QMBT::Logger::GetClientLogger()->critical(__VA_ARGS__)
