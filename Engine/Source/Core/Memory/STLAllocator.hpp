#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"

namespace QMBT
{
	class STLAllocator
	{
	  public:
		STLAllocator(const char* debugName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
		STLAllocator(const STLAllocator& other);

		STLAllocator& operator=(const STLAllocator& other);

		void* allocate(Size numBytes, int flags = 0);
		void* allocate(Size numBytes, Size alignment, Size offset, int flags = 0);
		void deallocate(void* ptr, Size numBytes);

	  protected:
		const char* m_DebugName;
	};

} // namespace QMBT