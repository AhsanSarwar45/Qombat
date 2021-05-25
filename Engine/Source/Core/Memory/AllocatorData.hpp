#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Core/Types/SharedPtr.hpp"
#include "Core/Types/String.hpp"
#include "Core/Types/UnorderedMap.hpp"

namespace QMBT
{
	struct AllocatorData
	{
		const char* DebugName;
		Size TotalSize;
		Size UsedSize;
		UnorderedMap<String, Size> Allocations;

		AllocatorData(const char* debugName, Size totalSize)
			: DebugName(debugName), TotalSize(totalSize), UsedSize(0)
		{
		}
	};

	using AllocatorVector = eastl::vector<SharedPtr<AllocatorData>>;

	enum class ResizePolicy : UInt8
	{
		Fixed,
		Resizable
	};

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
} // namespace QMBT