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
		// StackAllocator(const StackAllocator&) = delete;
		// StackAllocator(StackAllocator&&) = delete;

		// StackAllocator& operator=(const StackAllocator&) = delete;
		// StackAllocator& operator=(StackAllocator&&) = delete;

		StackAllocator(const std::string& debugName = "Allocator", const Size totalSize = 50_MB);

		~StackAllocator();

		void* Allocate(const Size size, const Size alignment = 8);

		template <typename Object, typename... Args>
		Object* New(Args... argList);

		template <typename Object>
		void Deallocate(Object* ptr);

	  private:
		//StackAllocator(StackAllocator& stackAllocator); //Restrict copying

		Ref<AllocatorData> m_Data;

		void* m_HeadPtr = nullptr;
		Size m_Offset;

		struct AllocationHeader
		{
			char padding;
		};
	};

	template <typename Object, typename... Args>
	Object* StackAllocator::New(Args... argList)
	{
		void* address = Allocate(sizeof(Object));
		return new (address) Object(argList...);
	}

	template <typename Object>
	void StackAllocator::Deallocate(Object* ptr)
	{
		const Size initialOffset = m_Offset;
		// Move offset back to clear address
		const Size currentAddress = (Size)ptr;
		const Size headerAddress = currentAddress - sizeof(AllocationHeader);
		const AllocationHeader* allocationHeader{(AllocationHeader*)headerAddress};

		m_Offset = currentAddress - allocationHeader->padding - (Size)m_HeadPtr;
		m_Data->UsedSize = m_Offset;

		ptr->~Object();
		LOG_MEMORY_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, initialOffset - m_Offset);
	}

} // namespace QMBT