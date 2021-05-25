#pragma once

namespace QMBT
{

	class STLAllocator
	{
	  public:
		STLAllocator(const char* debugName = "STL Allocator");
		STLAllocator(const STLAllocator& other);

		STLAllocator& operator=(const STLAllocator& other);

		void* allocate(size_t numBytes, int flags = 0);
		void* allocate(size_t numBytes, size_t alignment, size_t offset, int flags = 0);
		void deallocate(void* ptr, size_t numBytes);

	  protected:
		const char* m_DebugName;
	};

} // namespace QMBT