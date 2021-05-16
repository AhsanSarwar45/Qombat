#pragma once

#include <QMBTPCH.hpp>

#include "AllocatorData.hpp"
#include "Core/Aliases.hpp"

namespace QMBT
{
	class VectorAllocatorGlobal
	{
	  public:
		VectorAllocatorGlobal(const char* debugName);
		~VectorAllocatorGlobal();

		void* allocate(size_t num_of_bytes, int flags = 0);
		void* allocate(size_t num_of_bytes, size_t alignment, size_t offset, int flags = 0);
		void deallocate(void* ptr, size_t num_of_bytes);

		static void RegisterData();

		inline Size GetUsedSize() const { return m_Data->UsedSize; }

	  protected:
		static Ref<AllocatorData> m_Data;
	};

} // namespace QMBT