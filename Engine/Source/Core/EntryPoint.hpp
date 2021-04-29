// Pragma once is used instead of header guards throught the codebase as
// they are less prone to bugs and is more convenient. The downside is that
// we cannot have multiple files with the same name across our project
#pragma once

#include "Core/Core.hpp"
#include "Core/Testing.hpp"

/*
This functions is defined in the client. It will
return a new instance of a custom class inherited from Application.
*/
extern QMBT::Application* QMBT::CreateApplication();

void LogSystemInfo()
{
	LOG_CORE_WARN("===========================================");
	LOG_CORE_WARN("SYSTEM INFORMATION");
	LOG_CORE_WARN("===========================================");
	LOG_CORE_WARN("Operating System: 	{0}", GetOSName());
	LOG_CORE_WARN("Cache Line Size: 	{0} bytes", GetCacheLineSize());
	LOG_CORE_WARN("===========================================");
}

int main(int argc, char* argv[])
{
	QMBT::Logger::InitializeEngineLoggers();

	//QMBT::Test();

	LogSystemInfo();

	auto app = QMBT::CreateApplication();

	QMBT_CORE_ASSERT(app, "App was not initialized properly!")
	app->Run();

	delete app;
}