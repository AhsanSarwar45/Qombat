#pragma once

#include <memory>

// #ifdef QMBT_DEBUG
// 	#define XENOS_ENABLE_ASSERTS
// #endif

// #ifdef  XENOS_ENABLE_ASSERTS
// 	#define XS_ASSERT(x, ...) {if(!(x)) {XS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
// 	#define XS_CORE_ASSERT(x, ...) {if(!(x)) {XS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
// #else
// 	#define XS_ASSERT(x, ...)
// 	#define XS_CORE_ASSERT(x, ...)
// #endif //  XENOS_ENABLE_ASSERTS

#define BIT(x) (1 << x)
#define NON_COPYABLE(Type)       \
	Type(const Type &) = delete; \
	Type &operator=(const Type &) = delete

#define QMBT_BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

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