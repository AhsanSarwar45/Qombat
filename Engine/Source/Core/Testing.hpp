// #pragma once
// #include "Core/Core.hpp"

// #include "Core/Configuration/ConfigManager.hpp"
// #include "Core/Memory/PoolAllocator.hpp"
// #include "Core/Memory/StackAllocator.hpp"
// #include "Utility/Hashing.hpp"
// #include <Core/Configuration/Configuration.hpp>

// namespace QMBT
// {

// 	class MemoryLogger
// 	{
// 	  public:
// 		static void Init()
// 		{
// 			CONFIG_GROUP("TestGroup",
// 						 CONFIG_INT(s_VerbosityLevel, "Verbosity Level", "Sets the Verbosity Level", 0, 3))
// 		}

// 		//inline static std::shared_ptr<spdlog::logger>& Get() { return s_Logger; }
// 		//inline static std::shared_ptr<spdlog::logger> s_Logger;
// 		inline static int s_VerbosityLevel = 5;
// 		inline static int s_Pattern = 3;
// 	};

// 	struct TestingPools
// 	{

// 		char a = 'g';
// 		double b = 5;
// 		int c;
// 	};

// 	void TestHash()
// 	{
// 		StringHash hash = StringHash("Arhedzejtgfxryeuhtrjytdkytkytjtfxjtrfrdjtyk");
// 		StringHash hash1 = StringHash("Arhedzejtgfxryeuhtrjytdkytkytjtfxjtrfrdjtyj");
// 		StringHash hash2 = StringHash("Arhedzejtgfxryeuhtrjytdkytkytjtfxjtrfrdjtyk");

// 		LOG_CORE_INFO("test hash = {0}", hash);
// 		LOG_CORE_INFO("test hash1 = {0}", hash1);
// 		LOG_CORE_INFO("test hash2 = {0}", hash2);
// 	}

// 	void TestMemory()
// 	{
// 		// StackAllocator allocator = StackAllocator("Stack Allocator");

// 		// TestingPools* objects[10];
// 		// for (int i = 0; i < 8; ++i)
// 		// {
// 		// 	objects[i] = allocator.Allocate<TestingPools>();
// 		// 	objects[i]->a = 'a' + i;
// 		// 	objects[i]->b = i;
// 		// }

// 		// for (int i = 0; i < 8; ++i)
// 		// {
// 		// 	LOG_CORE_INFO("Info {0}, {1}, {2}", objects[i]->a, objects[i]->b, objects[i]->c);
// 		// }

// 		// for (int i = 7; i >= 0; --i)
// 		// {
// 		// 	allocator.Deallocate<TestingPools>(objects[i]);
// 		// }
// 	}

// 	void Test()
// 	{
// 		LOG_CORE_INFO("Initial Val: {0}", MemoryLogger::s_VerbosityLevel);
// 		MemoryLogger::Init();
// 		ConfigManager::SetConfigInt("TestGroup", "Verbosity Level", 345);
// 		LOG_CORE_INFO("Initial Val: {0}", MemoryLogger::s_VerbosityLevel);
// 	}

// } // namespace QMBT