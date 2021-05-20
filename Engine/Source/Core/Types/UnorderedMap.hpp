#pragma once

#include <QMBTPCH.hpp>

namespace QMBT
{
	template <typename Key, typename T>
	using UnorderedMap = eastl::unordered_map<Key, T>;
}
