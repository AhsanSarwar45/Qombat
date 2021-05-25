#include "CoreConfig.hpp"
#include "Memory/FreeListAllocator.hpp"
#include "Memory/STLAllocator.hpp"

namespace QMBT
{
	FreeListAllocator g_GlobalAllocator;
	FreeListAllocator* g_GlobalAllocatorPtr = &g_GlobalAllocator;

	STLAllocator g_DefaultSTLAllocator;
	STLAllocator* g_DefaultSTLAllocatorPtr = &g_DefaultSTLAllocator;

	FreeListAllocator* GetGlobalAllocator()
	{
		return g_GlobalAllocatorPtr;
	}
} // namespace QMBT

QMBT::STLAllocator* GetGlobalSTLAllocator()
{
	return QMBT::g_DefaultSTLAllocatorPtr;
}