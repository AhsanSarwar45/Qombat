#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/VectorAllocator.hpp"
#include "Core/Memory/VectorAllocatorGlobal.hpp"

namespace QMBT
{
	template <typename T, typename Allocator = VectorAllocatorGlobal>
	using Vector = eastl::vector<T, Allocator>;

	template <typename T, Size N>
	using Array = eastl::array<T, N>;
}
