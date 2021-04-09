// #pragma once

// #include <QMBTPCH.hpp>

// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

// #include <spdlog/fmt/ostr.h>
// #include <spdlog/spdlog.h>

// #include <Core/Aliases.hpp>

// namespace QMBT
// {

// }

// // namespace QMBT
// // {
// // 	class Console; // Forward declaration

// // 	using LoggerVector = std::vector < std::shared_ptr<spdlog::logger>>;

// // 	enum class LoggerCategory : UInt16
// // 	{
// // 		Core = 0,
// // 		Memory = 1,
// // 		Application = 2
// // 	};

// // 	class Logger
// // 	{
// // 	  public:
// // 		static void InitializeCoreLogger();
// // 		static void InitializeClientLogger(Ref<Console> console);

// // 		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
// // 		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

// // 		static void SetVerbosity(bool verbosity);

// // 	  private:
// // 		static std::shared_ptr<spdlog::logger> s_CoreLogger;
// // 		static std::shared_ptr<spdlog::logger> s_ClientLogger;

// // 		static const char* s_Pattern;
// // 		static const char* s_VerbosePattern;

// // 		static UInt8 s_VerbosityLevel;
// // 	};
// // } // namespace QMBT

// // // Core log macros for the different log levels
// // #define LOG_CORE_TRACE(...)          \
// // 	if (::QMBT::Logger::s_LogCoreOn) \
// // 	SPDLOG_LOGGER_TRACE(::QMBT::Logger::GetCoreLogger(), __VA_ARGS__)
// // #define LOG_CORE_INFO(...)           \
// // 	if (::QMBT::Logger::s_LogCoreOn) \
// // 	SPDLOG_LOGGER_INFO(::QMBT::Logger::GetCoreLogger(), __VA_ARGS__)
// // #define LOG_CORE_WARN(...)           \
// // 	if (::QMBT::Logger::s_LogCoreOn) \
// // 	SPDLOG_LOGGER_WARN(::QMBT::Logger::GetCoreLogger(), __VA_ARGS__)
// // #define LOG_CORE_CRITICAL(...)       \
// // 	if (::QMBT::Logger::s_LogCoreOn) \
// // 	SPDLOG_LOGGER_CRITICAL(::QMBT::Logger::GetCoreLogger(), __VA_ARGS__)
// // #define LOG_CORE_ERROR(...)          \
// // 	if (::QMBT::Logger::s_LogCoreOn) \
// // 	SPDLOG_LOGGER_ERROR(::QMBT::Logger::GetCoreLogger(), __VA_ARGS__)

// // #define LOG_MEMORY_INFO(...)           \
// // 	if (::QMBT::Logger::s_LogMemoryOn) \
// // 	SPDLOG_LOGGER_INFO(::QMBT::Logger::GetMemoryLogger(), __VA_ARGS__)
// // #define LOG_MEMORY_WARN(...)           \
// // 	if (::QMBT::Logger::s_LogMemoryOn) \
// // 	SPDLOG_LOGGER_WARN(::QMBT::Logger::GetMemoryLogger(), __VA_ARGS__)
// // #define LOG_MEMORY_TRACE(...)          \
// // 	if (::QMBT::Logger::s_LogMemoryOn) \
// // 	SPDLOG_LOGGER_TRACE(::QMBT::Logger::GetMemoryLogger(), __VA_ARGS__)
// // #define LOG_MEMORY_CRITICAL(...)       \
// // 	if (::QMBT::Logger::s_LogMemoryOn) \
// // 	SPDLOG_LOGGER_CRITICAL(::QMBT::Logger::GetMemoryLogger(), __VA_ARGS__)
// // #define LOG_MEMORY_ERROR(...)          \
// // 	if (::QMBT::Logger::s_LogMemoryOn) \
// // 	SPDLOG_LOGGER_ERROR(::QMBT::Logger::GetMemoryLogger(), __VA_ARGS__)

// // #define LOG_APP_TRACE(...)          \
// // 	if (::QMBT::Logger::s_LogAppOn) \
// // 	SPDLOG_LOGGER_TRACE(::QMBT::Logger::GetAppLogger(), __VA_ARGS__)
// // #define LOG_APP_INFO(...)           \
// // 	if (::QMBT::Logger::s_LogAppOn) \
// // 	SPDLOG_LOGGER_INFO(::QMBT::Logger::GetAppLogger(), __VA_ARGS__)
// // #define LOG_APP_WARN(...)           \
// // 	if (::QMBT::Logger::s_LogAppOn) \
// // 	SPDLOG_LOGGER_WARN(::QMBT::Logger::GetAppLogger(), __VA_ARGS__)
// // #define LOG_APP_CRITICAL(...)       \
// // 	if (::QMBT::Logger::s_LogAppOn) \
// // 	SPDLOG_LOGGER_CRITICAL(::QMBT::Logger::GetAppLogger(), __VA_ARGS__)
// // #define LOG_APP_ERROR(...)          \
// // 	if (::QMBT::Logger::s_LogAppOn) \
// // 	SPDLOG_LOGGER_ERROR(::QMBT::Logger::GetAppLogger(), __VA_ARGS__)

// // // Client log macros
// // #define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::QMBT::Logger::GetClientLogger(), __VA_ARGS__)
// // #define LOG_INFO(...) SPDLOG_LOGGER_INFO(::QMBT::Logger::GetClientLogger(), __VA_ARGS__)
// // #define LOG_WARN(...) SPDLOG_LOGGER_WARN(::QMBT::Logger::GetClientLogger(), __VA_ARGS__)
// // #define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::QMBT::Logger::GetClientLogger(), __VA_ARGS__)
// // #define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::QMBT::Logger::GetClientLogger(), __VA_ARGS__)
