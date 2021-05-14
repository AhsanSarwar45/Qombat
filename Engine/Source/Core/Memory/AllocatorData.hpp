#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"

namespace QMBT
{
	struct AllocatorData
	{
		std::string DebugName;
		Size TotalSize;
		Size UsedSize;

		AllocatorData(const std::string debugName, Size totalSize)
			: DebugName(debugName), TotalSize(totalSize), UsedSize(0) {}
	};

	using AllocatorVector = std::vector<Ref<AllocatorData>>;
}