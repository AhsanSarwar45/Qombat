#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Core/Types/SharedPtr.hpp"
#include "Core/Types/String.hpp"
#include "Core/Types/UnorderedMap.hpp"

namespace QMBT
{
	/**
	 * @brief A structure to store debug informations regarding each allocator.
	 * TODO: Make it not be used in Release builds
	 * 
	 */
	struct AllocatorData
	{
		const char* DebugName;
		Size TotalSize;
		Size UsedSize;
		std::unordered_map<std::string, Size> Allocations;

		AllocatorData(const char* debugName, Size totalSize)
			: DebugName(debugName), TotalSize(totalSize), UsedSize(0)
		{
		}
	};

	using AllocatorVector = std::vector<std::shared_ptr<AllocatorData>>;

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