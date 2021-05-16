#include "VectorAllocator.hpp"

#include "Core/Logging/Logger.hpp"
#include "MemoryManager.hpp"

namespace QMBT
{
	VectorAllocator::VectorAllocator(const char* debugName)
	{
		m_Data = CreateRef<AllocatorData>(debugName, 0);
		MemoryManager::GetInstance().Register(m_Data);
		LOG_MEMORY_INFO("Initialized {0}", debugName);
	}

	VectorAllocator::VectorAllocator(const VectorAllocator& other)
		: m_Data(other.m_Data)
	{
		//m_Data = CreateRef<AllocatorData>(other.m_Data->DebugName, other.m_Data->TotalSize);
	}

	VectorAllocator::~VectorAllocator()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
	}

	VectorAllocator& VectorAllocator::operator=(const VectorAllocator& x)
	{
		m_Data = x.m_Data;
		return *this;
	}

	void* VectorAllocator::allocate(size_t num_of_bytes, int flags)
	{
		m_Data->TotalSize += num_of_bytes;
		void* ptr = ::new ((char*)0, flags, 0, (char*)0, 0) char[num_of_bytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes", m_Data->DebugName, num_of_bytes);
		return ptr;
	}
	void* VectorAllocator::allocate(size_t num_of_bytes, size_t alignment, size_t offset, int flags)
	{
		m_Data->TotalSize += num_of_bytes;
		void* ptr = ::new (alignment, offset, (char*)0, flags, 0, (char*)0, 0) char[num_of_bytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes with alignment {2}", m_Data->DebugName, num_of_bytes, alignment);
		return ptr;
	}
	void VectorAllocator::deallocate(void* ptr, size_t num_of_bytes)
	{
		m_Data->TotalSize -= num_of_bytes;
		LOG_MEMORY_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, num_of_bytes);
		delete[](char*) ptr;
	}

	bool operator==(const VectorAllocator& a, const VectorAllocator& b)
	{
		if (&a == &b)
		{
			return true; // allocator a and b are equal if they are the same
		}
		else
		{
			return false; // otherwise, return false, because the state m_totalAmountOfBytesAllocated needs to be increased/decreased on splice and swap
		}
	}
	bool operator!=(const VectorAllocator& a, const VectorAllocator& b)
	{
		return false;
	}
} // namespace QMBT