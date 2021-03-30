#pragma once

#include <QMBTPCH.hpp>
#include "Compatibility/DebugBreak.hpp"
#include "Compatibility/PlatformDetection.hpp"

//Contains common Aliases, Macros and Helper Functions.

namespace QMBT
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args &&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using WeakRef = std::weak_ptr<T>;
	template <typename T, typename... Args>
	constexpr WeakRef<T> CreateWeakRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	bool IsUninitialized(WeakRef<T> const &weak)
	{
		return !weak.owner_before(WeakRef<T>{}) && !WeakRef<T>{}.owner_before(weak);
	}
}

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

#define NON_COPYABLE(Type)       \
	Type(const Type &) = delete; \
	Type &operator=(const Type &) = delete

#define QMBT_BIND_EVENT_FUNCTION(function) [this](auto &&...args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }
