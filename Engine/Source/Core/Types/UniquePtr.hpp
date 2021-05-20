#pragma once

#include <QMBTPCH.hpp>

// #include "Core/Memory/STLAllocator.hpp"

namespace QMBT
{
	template <typename T>
	using UniquePtr = eastl::unique_ptr<T>;

	// template <typename T, typename... Args>
	// constexpr UniquePtr<T> AllocateShared(Args&&... args)
	// {
	// 	return std::make_shared<T>(std::forward<Args>(args)...);
	// }

	template <typename T, typename... Args>
	constexpr UniquePtr<T> MakeUnique(Args&&... args)
	{
		return eastl::make_unique<T>(std::forward<Args>(args)...);
	}
} // namespace QMBT