#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/MemoryManager.hpp"
#include "Core/Memory/Utility/MemoryUtils.hpp"

namespace QMBT
{

	class StackAllocator
	{
	  public:
		StackAllocator() = delete;
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;

		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator&&) = delete;

		StackAllocator(const std::string& debugName = "Allocator", const Size totalSize = 50_MB);

		~StackAllocator();

		template <typename Object>
		Object* Allocate(const Size alignment = 8);

		template <typename Object>
		void Deallocate(Object* chunk);

	  private:
		StackAllocator(StackAllocator& stackAllocator); //Restrict copying

		Ref<AllocatorData> m_Data;

		void* m_HeadPtr = nullptr;
		Size m_Offset;

		struct AllocationHeader
		{
			char padding;
		};
	};

	template <typename Object>
	Object* StackAllocator::Allocate(const Size alignment)
	{
		const Size size = sizeof(Object);
		const Size currentAddress = (Size)m_HeadPtr + m_Offset;

		Size padding = Utils::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

		if (m_Offset + padding + size > m_Data->TotalSize)
		{
			LOG_CORE_CRITICAL("{0}: Allocation exceeded maximum size of {1}!", m_Data->DebugName, m_Data->TotalSize);
			return nullptr;
		}
		m_Offset += padding;

		const Size nextAddress = currentAddress + padding;
		const Size headerAddress = nextAddress - sizeof(AllocationHeader);
		AllocationHeader allocationHeader{padding};
		AllocationHeader* headerPtr = (AllocationHeader*)headerAddress;
		headerPtr = &allocationHeader;

		m_Offset += size;

		m_Data->UsedSize = m_Offset;
		LOG_CORE_INFO("{0} Allocated {1} bytes with alignment {2}", m_Data->DebugName, size, alignment);
		return (Object*)nextAddress;
	}

	template <typename Object>
	void StackAllocator::Deallocate(Object* chunk)
	{
		const Size initialOffset = m_Offset;
		// Move offset back to clear address
		const Size currentAddress = (Size)chunk;
		const Size headerAddress = currentAddress - sizeof(AllocationHeader);
		const AllocationHeader* allocationHeader{(AllocationHeader*)headerAddress};

		m_Offset = currentAddress - allocationHeader->padding - (Size)m_HeadPtr;
		m_Data->UsedSize = m_Offset;

		LOG_CORE_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, initialOffset - m_Offset);
	}

} // namespace QMBT