#pragma once

#include <QMBTPCH.hpp>

#include "Core/Compatibility/DebugBreak.hpp"
#include "Core/Compatibility/PlatformDetection.hpp"

#include "Core/Aliases.hpp"
#include "Core/Logger.hpp"

// Contains common Aliases, Macros and Helper Functions.

#define OVERLOAD_NEW(name) \
	// inline void* operator new(size_t size)                                           \
	// {                                                                                \
	// 	LOG_CORE_TRACE("Allocated {0}: {1} bytes", name, size);                      \
	// 	return malloc(size * sizeof(char));                                          \
	// }                                                                                \
	// inline void operator delete(void* ptr) noexcept                                  \
	// {                                                                                \
	// 	LOG_CORE_TRACE("Freed {0}", name);                                           \
	// 	std::free(ptr);                                                              \
	// }                                                                                \
	// inline void* operator new[](size_t size) { return malloc(size * sizeof(char)); } \
	// inline void operator delete[](void* ptr) noexcept { std::free(ptr); }

#ifdef QMBT_DEBUG
#define QMBT_ENABLE_ASSERTS
#define QMBT_ENABLE_VERIFICATION
#endif

#ifdef QMBT_ENABLE_ASSERTS

#define QMBT_ASSERT(x, ...)                                  \
	{                                                        \
		if (!(x))                                            \
		{                                                    \
			LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			debug_break();                                   \
		}                                                    \
	}
#define QMBT_CORE_ASSERT(x, ...)                                  \
	{                                                             \
		if (!(x))                                                 \
		{                                                         \
			LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			debug_break();                                        \
		}                                                         \
	}
#else
#define QMBT_ASSERT(x, ...)
#define QMBT_CORE_ASSERT(x, ...)
#endif //  QMBT_ENABLE_ASSERTS

#ifdef QMBT_ENABLE_VERIFICATION

#define QMBT_VERIFY(x, ...)                                     \
	{                                                           \
		if (!(x))                                               \
		{                                                       \
			LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
			debug_break();                                      \
		}                                                       \
	}
#define QMBT_CORE_VERIFY(x, ...)                                     \
	{                                                                \
		if (!(x))                                                    \
		{                                                            \
			LOG_CORE_ERROR("Verification Failed: {0}", __VA_ARGS__); \
			debug_break();                                           \
		}                                                            \
	}
#else
#define QMBT_VERIFY(x, ...) x
#define QMBT_CORE_VERIFY(x, ...) x
#endif //  QMBT_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define NON_COPYABLE(Type)      \
	Type(const Type&) = delete; \
	Type& operator=(const Type&) = delete

#define BIND_EVENT_FUNCTION(function)                                 \
	[this](auto&&... args) -> decltype(auto) {                        \
		return this->function(std::forward<decltype(args)>(args)...); \
	}
