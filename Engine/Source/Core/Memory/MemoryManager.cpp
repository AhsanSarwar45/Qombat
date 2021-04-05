#include "MemoryManager.hpp"

namespace QMBT
{
	MemoryManager& MemoryManager::GetInstance()
	{
		static MemoryManager* s_MemoryManager = nullptr;
		if (s_MemoryManager == nullptr)
		{
			s_MemoryManager = new MemoryManager(MB(500));
			LOG_MEMORY_INFO("Instantiated Memory Manager with total memory budget of {0} bytes ({1} MB)",
							s_MemoryManager->GetApplicationMemoryBudget(),
							ToMB(s_MemoryManager->GetApplicationMemoryBudget()));
		}
		QMBT_CORE_ASSERT(s_MemoryManager, "Memory Manager not initialized properly!");
		return *s_MemoryManager;
	}

	void MemoryManager::Register(Ref<AllocatorData> allocatorData)
	{
		m_TotalSizeAllocated += allocatorData->TotalSize;

		LOG_MEMORY_INFO("Registering Allocator of total size {0} bytes ({1} MB)",
						allocatorData->TotalSize,
						ToMB(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated increased to  {0} bytes ({1} MB). Total budget left is {2} bytes ({3} MB)",
						m_TotalSizeAllocated,
						ToMB(m_TotalSizeAllocated),
						m_ApplicationBudget - m_TotalSizeAllocated,
						ToMB(m_ApplicationBudget - m_TotalSizeAllocated));
		QMBT_CORE_ASSERT(m_TotalSizeAllocated < m_ApplicationBudget, "Exceeded application memory budget!")
		allocators.push_back(allocatorData);
	}

	void MemoryManager::UnRegister(Ref<AllocatorData> allocatorData)
	{
		allocators.erase(std::remove(allocators.begin(), allocators.end(), allocatorData), allocators.end());

		m_TotalSizeAllocated -= allocatorData->TotalSize;

		LOG_MEMORY_INFO("UnRegistering Allocator of total size {0} bytes ({1} MB)",
						allocatorData->TotalSize,
						ToMB(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated decreased to  {0} bytes ({1} MB). Total budget left is {2} bytes ({3} MB)",
						m_TotalSizeAllocated,
						ToMB(m_TotalSizeAllocated),
						m_ApplicationBudget - m_TotalSizeAllocated,
						ToMB(m_ApplicationBudget - m_TotalSizeAllocated));
	}

	Size MemoryManager::GetUsedAllocatedSize() const
	{
		Size usedSize = 0;
		for (const auto& it : allocators)
		{
			usedSize += it->UsedSize;
		}

		return usedSize;
	}
} // namespace QMBT