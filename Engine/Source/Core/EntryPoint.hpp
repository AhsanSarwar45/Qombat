// Pragma once is used instead of header guards throught the codebase as
// they are less prone to bugs and is more convenient. The downside is that
// we cannot have multiple files with the same name across our project
#pragma once

#include "Core/Core.hpp"
// #include "Core/Memory/PoolAllocator.hpp"
// #include "Core/Memory/StackAllocator.hpp"

/*
This functions is defined in the client. It will
return a new instance of a custom class inherited from Application.
*/
extern QMBT::Application*
QMBT::CreateApplication();

// namespace QMBT
// {
// 	struct TestingPools
// 	{

// 		char a = 'g';
// 		double b = 5;
// 		int c;
// 	};

// 	void TestMemory()
// 	{
// 		StackAllocator allocator = StackAllocator("Stack Allocator");

// 		TestingPools* objects[10];
// 		for (int i = 0; i < 8; ++i)
// 		{
// 			objects[i] = allocator.Allocate<TestingPools>();
// 			objects[i]->a = 'a' + i;
// 			objects[i]->b = i;
// 		}

// 		for (int i = 0; i < 8; ++i)
// 		{
// 			LOG_CORE_INFO("Info {0}, {1}, {2}", objects[i]->a, objects[i]->b, objects[i]->c);
// 		}

// 		for (int i = 7; i >= 0; --i)
// 		{
// 			allocator.Deallocate<TestingPools>(objects[i]);
// 		}
// 	}

// } // namespace QMBT

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

	//QMBT::TestMemory();

	LogSystemInfo();

	auto app = QMBT::CreateApplication();

	QMBT_CORE_ASSERT(app, "App was not initialized properly!")
	app->Run();

	delete app;
}