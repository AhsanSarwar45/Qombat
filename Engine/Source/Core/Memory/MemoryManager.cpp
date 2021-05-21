#include "MemoryManager.hpp"

#include "Core/Core.hpp"
#include "Utility/Size.hpp"

namespace QMBT
{
	MemoryManager::MemoryManager(Size applicationBudget)
		: m_ApplicationBudget(applicationBudget), m_TotalAllocatedSize(0)
	{
		LOG_MEMORY_INFO("Instantiated Memory Manager with total memory budget of {0}",
						Utility::ToReadable(m_ApplicationBudget));
	}

	MemoryManager& MemoryManager::GetInstance()
	{
		static MemoryManager* s_MemoryManager = nullptr;
		if (!s_MemoryManager)
		{
			s_MemoryManager = new MemoryManager(500_MB);
		}
		return *s_MemoryManager;
	}

	void MemoryManager::Register(SharedPtr<AllocatorData> allocatorData)
	{
		m_TotalAllocatedSize += allocatorData->TotalSize;

		LOG_MEMORY_INFO("Registering {0} of total size {1}",
						allocatorData->DebugName,
						Utility::ToReadable(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated increased to {0}. Total budget left is {1}",
						Utility::ToReadable(m_TotalAllocatedSize),
						Utility::ToReadable(m_ApplicationBudget - m_TotalAllocatedSize));
		QMBT_CORE_ASSERT(m_TotalAllocatedSize < m_ApplicationBudget, "Exceeded application memory budget!")
		m_Allocators.push_back(allocatorData);
	}

	void MemoryManager::UnRegister(SharedPtr<AllocatorData> allocatorData)
	{
		m_Allocators.erase(std::remove(m_Allocators.begin(), m_Allocators.end(), allocatorData), m_Allocators.end());

		m_TotalAllocatedSize -= allocatorData->TotalSize;

		LOG_MEMORY_INFO("UnRegistering {0} of total size {1}",
						allocatorData->DebugName,
						Utility::ToReadable(allocatorData->TotalSize));
		LOG_MEMORY_INFO("Total size allocated decreased to  {0}. Total budget left is {1}",
						Utility::ToReadable(m_TotalAllocatedSize),
						Utility::ToReadable(m_ApplicationBudget - m_TotalAllocatedSize));
	}

	Size MemoryManager::GetUsedAllocatedSize() const
	{
		Size usedSize = 0;
		for (const auto& it : m_Allocators)
		{
			usedSize += it->UsedSize;
		}

		return usedSize;
	}
} // namespace QMBT