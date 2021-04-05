#include "Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <Core/Console.hpp>

namespace QMBT
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_MemoryLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	bool Logger::s_LogCoreOn = true;
	bool Logger::s_LogMemoryOn = false;
	bool Logger::s_LogAppOn = true;

	const char* Logger::s_Pattern = "%^[%T] %n: %v%$";
	const char* Logger::s_VerbosePattern = "%^[%T][%s:%#] %n: %v%$";

	bool Logger::s_Verbose = true;

	void Logger::InitializeEngineLoggers()
	{
		s_CoreLogger = spdlog::stdout_color_mt("QMBT");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_MemoryLogger = spdlog::stdout_color_mt("Memory");
		s_MemoryLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("App");
		s_AppLogger->set_level(spdlog::level::trace);

		if (s_Verbose)
		{
			s_CoreLogger->set_pattern(s_VerbosePattern);
			s_MemoryLogger->set_pattern(s_VerbosePattern);
			s_AppLogger->set_pattern(s_VerbosePattern);
		}
		else
		{
			s_CoreLogger->set_pattern(s_Pattern);
			s_MemoryLogger->set_pattern(s_Pattern);
			s_AppLogger->set_pattern(s_Pattern);
		}

		LOG_CORE_INFO("Initialized Core Logger");
	}

	void Logger::InitializeClientLogger(Ref<Console> console)
	{
		s_ClientLogger = spdlog::ConsoleSink_mt("App", console);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_pattern(s_Pattern);

		LOG_CORE_INFO("Initialized Client Logger");
	}

	void Logger::SetVerbosity(bool verbosity)
	{
		if (verbosity && !s_Verbose)
		{
			s_CoreLogger->set_pattern(s_VerbosePattern);
			s_MemoryLogger->set_pattern(s_VerbosePattern);
			s_AppLogger->set_pattern(s_VerbosePattern);
			s_ClientLogger->set_pattern(s_VerbosePattern);
		}

		s_Verbose = verbosity;
	}

	// Note: To add multiple sinks, see https://github.com/gabime/spdlog/wiki/2.-Creating-loggers
	// under section "Creating loggers with multiple sinks"
} // namespace QMBT