#pragma once

#include <QMBTPCH.hpp>

// #include "Core/Memory/STLAllocator.hpp"

namespace QMBT
{
	template <typename T>
	using SharedPtr = eastl::shared_ptr<T>;
	template <typename T>
	using WeakPtr = eastl::weak_ptr<T>;

	template <typename T, typename Allocator, typename... Args>
	constexpr SharedPtr<T> AllocateShared(const Allocator& allocator, Args&&... args)
	{
		return eastl::allocate_shared<T>(allocator, std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	constexpr SharedPtr<T> MakeShared(Args&&... args)
	{
		return eastl::make_shared<T>(std::forward<Args>(args)...);
	}

} // namespace QMBT
