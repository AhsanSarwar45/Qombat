// #include "Core.hpp"
// #include "Core/Memory/FreeListAllocator.hpp"

// #define EASTL_DEFAULT_NAME_PREFIX "Qombat"

// #define EASTL_ASSERT_MSG(expression, message) QMBT_ASSERT(expression, message);

// #define EASTL_FAIL_MSG(message) LOG_CORE_ERROR(message);

// #define EASTLAlloc(allocator, n) (allocator).Allocate(n);

// #define EASTLAllocFlags(allocator, n, flags) (allocator).Allocate(n, flags);

// #define EASTLAllocAligned(allocator, n, alignment, offset) (allocator).Allocate((n), (alignment), (offset));

// #define EASTLAllocAlignedFlags(allocator, n, alignment, offset, flags) (allocator).Allocate((n), (alignment), (offset), (flags));

// #define EASTLFree(allocator, p, size) (allocator).Deallocate((void*)(p), (size));

// // template <>
// // inline ::QMBT::FreeListAllocator* get_default_allocator<::QMBT::FreeListAllocator>(const ::QMBT::FreeListAllocator*)
// // {
// // 	return ::QMBT::Application::GetFreeListAllocator(); // By default we return NULL; the user must make specialization of this function in order to provide their own implementation.
// // }