#pragma once
#include <deque>
#include <string>
#include <spdlog/spdlog.h>

#include "Core.hpp"

namespace QMBT
{
	using LogLevel = spdlog::level::level_enum;
	struct Log
	{
		std::string message;
		LogLevel level;

		Log(const std::string &_message, LogLevel _level)
			: message(_message), level(_level)
		{
		}
	};

	class Console
	{
	public:
		Console();
		Console(int maxLogs);

		void AddLog(const std::string &log, LogLevel level);
		inline void Clear() { std::deque<Ref<Log>>().swap(*m_Logs); }

		Ref<std::deque<Ref<Log>>> GetLogs() const { return m_Logs; }

	private:
		void Initialize();

	private:
		int m_MaxLogs;
		Ref<std::deque<Ref<Log>>> m_Logs;
	};
}
