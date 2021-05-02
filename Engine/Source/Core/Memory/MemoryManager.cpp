#include "MemoryManager.hpp"
#include "Utility/Size.hpp"

namespace QMBT
{
	MemoryManager& MemoryManager::GetInstance()
	{
		static MemoryManager* s_MemoryManager = nullptr;
		if (s_MemoryManager == nullptr)
		{
			s_MemoryManager = new MemoryManager(500_MB);
			LOG_MEMORY_INFO("Instantiated Memory Manager with total memory budget of {0}",
							Utility::ToReadable(s_MemoryManager->GetApplicationMemoryBudget()));
		}
		QMBT_CORE_ASSERT(s_MemoryManager, "Memory Manager not initialized properly!");
		return *s_MemoryManager;
	}

	void MemoryManager::Register(Ref<AllocatorData> allocatorData)
	{
		m_TotalAllocatedSize += allocatorData->TotalSize;

		LOG_MEMORY_INFO("Registering Allocator of total size {0}",
						Utility::ToReadable(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated increased to  {0}. Total budget left is {1}",
						Utility::ToReadable(m_TotalAllocatedSize),
						Utility::ToReadable(m_ApplicationBudget - m_TotalAllocatedSize));
		QMBT_CORE_ASSERT(m_TotalAllocatedSize < m_ApplicationBudget, "Exceeded application memory budget!")
		allocators.push_back(allocatorData);
	}

	void MemoryManager::UnRegister(Ref<AllocatorData> allocatorData)
	{
		allocators.erase(std::remove(allocators.begin(), allocators.end(), allocatorData), allocators.end());

		m_TotalAllocatedSize -= allocatorData->TotalSize;

		LOG_MEMORY_INFO("UnRegistering Allocator of total size {0}",
						allocatorData->TotalSize,
						Utility::ToReadable(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated decreased to  {0}. Total budget left is {1}",
						Utility::ToReadable(m_TotalAllocatedSize),
						Utility::ToReadable(m_ApplicationBudget - m_TotalAllocatedSize));
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