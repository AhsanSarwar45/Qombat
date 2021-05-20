#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "Core/Core.hpp"
#include "Core/Types/SharedPtr.hpp"
#include "Core/Types/String.hpp"

namespace QMBT
{

	using LogLevel = spdlog::level::level_enum;

	struct Log
	{
		std::string message;
		LogLevel level;

		Log(const std::string& _message, LogLevel _level)
			: message(_message), level(_level)
		{
		}
	};

	using Logs = std::deque<SharedPtr<Log>>;

	class Console
	{
	  public:
		Console();
		Console(int maxLogs);

		void AddLog(const std::string& log, LogLevel level);
		inline void Clear() { Logs().swap(*m_Logs); }

		inline SharedPtr<Logs> GetLogs() const { return m_Logs; }

	  private:
		void Initialize();

	  private:
		int m_MaxLogs;
		SharedPtr<Logs> m_Logs;
	};
} // namespace QMBT

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
		explicit ConsoleSink(QMBT::SharedPtr<QMBT::Console> console)
		{
			m_Console = console;
		}

	  protected:
		// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
		// msg.raw contains pre formatted log
		void sink_it_(const spdlog::details::log_msg& msg) override
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
		QMBT::SharedPtr<QMBT::Console> m_Console;
	};
} // namespace spdlog::sinks

namespace spdlog
{
	// A Factory for the multi threaded version of the CustomSink
	// Multi-threaded version is slower than a single-threaded version due to thread locking but is thread-safe
	template <typename Factory = synchronous_factory>
	inline std::shared_ptr<logger> ConsoleSink_mt(const std::string& logger_name, QMBT::SharedPtr<QMBT::Console> console)
	{
		return Factory::template create<sinks::ConsoleSink<std::mutex>>(logger_name, console);
	}

} // namespace spdlog
