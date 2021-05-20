#include "Console.hpp"

namespace QMBT
{

	Console::Console()
		: m_MaxLogs(50)
	{
		Initialize();
	}

	Console::Console(int maxLogs)
		: m_MaxLogs(maxLogs)
	{
		Initialize();
	}

	void Console::Initialize()
	{
		m_Logs = MakeShared<Logs>();
	}

	void Console::AddLog(const std::string& log, LogLevel level)
	{
		if (m_Logs->size() >= m_MaxLogs)
		{
			m_Logs->pop_front();
		}
		m_Logs->push_back(MakeShared<Log>(log, level));
	}

} // namespace QMBT
