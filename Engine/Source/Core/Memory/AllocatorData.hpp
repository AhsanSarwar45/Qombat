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

	using AllocatorVector = eastl::vector<Ref<AllocatorData>>;

	struct Chunk
	{
		/*
		When a chunk is free, the `next` contains the
		address of the next chunk in a list.
		
		When it's allocated, this space is used by
		the user.
		*/
		Chunk* next;
	};
}