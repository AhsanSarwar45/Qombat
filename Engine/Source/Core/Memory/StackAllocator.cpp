#include "StackAllocator.hpp"
#include "Core/Core.hpp"
#include "Utility/Size.hpp"

namespace QMBT
{
	StackAllocator::StackAllocator(const char* debugName, Size totalSize)
	{
		QMBT_CORE_ASSERT(totalSize < 1_GB && totalSize > 0, "Total size of allocator cannot be more than 1 GB or less than 0");

		m_Data = MakeShared<AllocatorData>(debugName, totalSize);

		// Allows the memory manager to keep track of total allocated memory
		MemoryManager::GetInstance().Register(m_Data);

		// Allocate all the memory upfront using malloc and store the address
		m_HeadPtr = malloc(m_Data->TotalSize);
		m_Offset = 0;

		LOG_MEMORY_INFO("Initialized {0} of size {1}", m_Data->DebugName, Utility::ToReadable(m_Data->TotalSize));
	}

	StackAllocator::~StackAllocator()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
		free(m_HeadPtr);
	}

	void* StackAllocator::Allocate(const Size size, const Size alignment)
	{
		const Size currentAddress = (Size)m_HeadPtr + m_Offset;

		Size padding = Utility::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

		if (m_Offset + padding + size > m_Data->TotalSize)
		{
			LOG_MEMORY_CRITICAL("{0}: Allocation exceeded maximum size of {1}!",
								m_Data->DebugName,
								Utility::ToReadable(m_Data->TotalSize));
			return nullptr;
		}
		m_Offset += padding;

		const Size nextAddress = currentAddress + padding;
		const Size headerAddress = nextAddress - sizeof(AllocationHeader);
		AllocationHeader allocationHeader{static_cast<unsigned char>(padding)};
		AllocationHeader* headerPtr = (AllocationHeader*)headerAddress;
		headerPtr = &allocationHeader;

		m_Offset += size;

		m_Data->UsedSize = m_Offset;
		LOG_MEMORY_INFO("{0} Allocated {1} bytes with alignment {2}", m_Data->DebugName, size, alignment);
		return (void*)nextAddress;
	}

	void StackAllocator::Deallocate(const Size ptr)
	{
		const Size initialOffset = m_Offset;

		// Move offset back to clear address
		const Size headerAddress = ptr - sizeof(AllocationHeader);
		const AllocationHeader* allocationHeader{(AllocationHeader*)headerAddress};

		m_Offset = ptr - allocationHeader->padding - (Size)m_HeadPtr;
		m_Data->UsedSize = m_Offset;

		LOG_MEMORY_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, Utility::ToReadable(initialOffset - m_Offset));
	}

} // namespace QMBT
