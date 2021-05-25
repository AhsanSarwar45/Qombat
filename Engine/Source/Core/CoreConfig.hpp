#include "Memory/STLAllocator.hpp"

namespace QMBT
{
	STLAllocator* GetDefaultGlobalAllocator();

	class FreeListAllocator;

	FreeListAllocator* GetGlobalAllocator();

} // namespace QMBT

#define EASTL_DEFAULT_NAME_PREFIX "Qombat"

#define EASTLAllocatorType QMBT::STLAllocator

#define EASTL_USER_DEFINED_ALLOCATOR

#define EASTLAllocatorDefault QMBT::GetDefaultGlobalAllocator

// #define EASTL_ASSERT_MSG(expression, message) QMBT_ASSERT(expression, message);

// #define EASTL_FAIL_MSG(message) LOG_CORE_ERROR(message);

// #define EASTLAlloc(allocator, n) (allocator).Allocate(n);

// #define EASTLAllocFlags(allocator, n, flags) (allocator).Allocate(n, flags);

// #define EASTLAllocAligned(allocator, n, alignment, offset) (allocator).Allocate((n), (alignment), (offset));

// #define EASTLAllocAlignedFlags(allocator, n, alignment, offset, flags) (allocator).Allocate((n), (alignment), (offset), (flags));

// #define EASTLFree(allocator, p, size) (allocator).Deallocate((void*)(p), (size));
