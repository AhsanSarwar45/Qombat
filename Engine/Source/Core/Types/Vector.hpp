#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/STLAllocator.hpp"

namespace QMBT
{
	template <typename T, typename Allocator = STLAllocator>
	using Vector = eastl::vector<T, Allocator>;

} // namespace QMBT