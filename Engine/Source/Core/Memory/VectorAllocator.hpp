#pragma once

#include <QMBTPCH.hpp>

#include "AllocatorData.hpp"
#include "Core/Aliases.hpp"

namespace QMBT
{
	class VectorAllocator
	{
	  public:
		EASTL_ALLOCATOR_EXPLICIT VectorAllocator(const char* debugName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
		VectorAllocator(const VectorAllocator& other);
		VectorAllocator(const VectorAllocator& other, const char* debugName);
		~VectorAllocator();

		VectorAllocator& operator=(const VectorAllocator& x);

		void* allocate(size_t num_of_bytes, int flags = 0);
		void* allocate(size_t num_of_bytes, size_t alignment, size_t offset, int flags = 0);
		void deallocate(void* ptr, size_t num_of_bytes);

		const char* get_name() const;
		void set_name(const char* debugName);

	  protected:
		Ref<AllocatorData> m_Data;
	};

} // namespace QMBT