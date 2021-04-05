#include "StackAllocator.hpp"

namespace QMBT
{
	StackAllocator::StackAllocator(const std::string& debugName, Size totalSize)
	{
		QMBT_CORE_ASSERT(totalSize < GB(1), "Total size of allocator cannot be larger than 1 GB");

		m_Data = std::make_shared<AllocatorData>(debugName, totalSize);

		MemoryManager::GetInstance().Register(m_Data);

		m_HeadPtr = malloc(m_Data->TotalSize);
		m_Offset = 0;

		LOG_CORE_INFO("Initialized {0} of size {1}", m_Data->DebugName, m_Data->TotalSize);
	}

	StackAllocator::~StackAllocator()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
		free(m_HeadPtr);
		m_HeadPtr = nullptr;
	}

} // namespace QMBT
