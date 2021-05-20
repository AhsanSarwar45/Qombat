#include "STLAllocator.hpp"

#include "Core/Application.hpp"
#include "Core/Logging/Logger.hpp"

namespace QMBT
{
	STLAllocator::STLAllocator(const char* debugName)
	{
		m_DebugName = debugName;
	}

	STLAllocator::STLAllocator(const STLAllocator& other)
	{
		m_DebugName = other.m_DebugName;
	}

	STLAllocator& STLAllocator::operator=(const STLAllocator& other)
	{
		m_DebugName = other.m_DebugName;
		return *this;
	}

	void* STLAllocator::allocate(size_t numBytes, int flags)
	{
		//void* ptr = ::new ((char*)0, flags, 0, (char*)0, 0) char[numBytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes", m_DebugName, numBytes);
		return Application::GetFreeListAllocator()->Allocate(numBytes, 8, m_DebugName);
	}
	void* STLAllocator::allocate(size_t numBytes, size_t alignment, size_t offset, int flags)
	{
		//void* ptr = ::new (alignment, offset, (char*)0, flags, 0, (char*)0, 0) char[numBytes];
		LOG_MEMORY_INFO("{0} Allocated {1} bytes with alignment {2}", m_DebugName, numBytes, alignment);
		return Application::GetFreeListAllocator()->Allocate(numBytes, alignment, m_DebugName);
	}
	void STLAllocator::deallocate(void* ptr, size_t numBytes)
	{
		LOG_MEMORY_INFO("{0} Deallocated {1} bytes", m_DebugName, numBytes);
		Application::GetFreeListAllocator()->Deallocate(ptr, m_DebugName);
	}

	bool operator==(const STLAllocator& a, const STLAllocator& b)
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
	bool operator!=(const STLAllocator& a, const STLAllocator& b)
	{
		return false;
	}
} // namespace QMBT