#include "VectorAllocatorGlobal.hpp"

#include "Core/Logging/Logger.hpp"
#include "MemoryManager.hpp"

namespace QMBT
{
	Ref<AllocatorData> VectorAllocatorGlobal::m_Data = CreateRef<AllocatorData>("Vector Allocator", 0);

	VectorAllocatorGlobal::VectorAllocatorGlobal(const char* debugName)
	{
		LOG_MEMORY_INFO("Initialized Vector Allocator");
	}

	void* VectorAllocatorGlobal::allocate(size_t numBytes, int flags)
	{
		m_Data->TotalSize += numBytes;
		void* ptr = ::new ((char*)0, flags, 0, (char*)0, 0) char[numBytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes", m_Data->DebugName, numBytes);
		return ptr;
	}
	void* VectorAllocatorGlobal::allocate(size_t numBytes, size_t alignment, size_t offset, int flags)
	{
		m_Data->TotalSize += numBytes;
		void* ptr = ::new (alignment, offset, (char*)0, flags, 0, (char*)0, 0) char[numBytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes with alignment {2}", m_Data->DebugName, numBytes, alignment);
		return ptr;
	}
	void VectorAllocatorGlobal::deallocate(void* ptr, size_t numBytes)
	{
		m_Data->TotalSize -= numBytes;
		LOG_MEMORY_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, numBytes);
		delete[](char*) ptr;
	}

	void VectorAllocatorGlobal::RegisterData()
	{
		MemoryManager::GetInstance().Register(m_Data);
	}

	const char* VectorAllocatorGlobal::get_name() const
	{
		return m_Data->DebugName.c_str();
	}
	void VectorAllocatorGlobal::set_name(const char* debugName)
	{
		m_Data->DebugName = debugName;
	}

	bool operator==(const VectorAllocatorGlobal& a, const VectorAllocatorGlobal& b)
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
	bool operator!=(const VectorAllocatorGlobal& a, const VectorAllocatorGlobal& b)
	{
		return false;
	}
} // namespace QMBT