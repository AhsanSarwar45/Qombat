#pragma once

#include <QMBTPCH.hpp>

#include "Core/Core.hpp"

namespace QMBT
{
	struct AllocatorData
	{
		std::string DebugName;
		Size TotalSize;
		Size UsedSize;

		AllocatorData(const std::string debugName, Size totalSize)
			: DebugName(debugName), TotalSize(totalSize), UsedSize(0) {}
	};

	class MemoryManager
	{
	  public:
		MemoryManager(Size applicationBudget)
			: m_ApplicationBudget(applicationBudget) {}
		~MemoryManager();

		static MemoryManager& GetInstance();

		void Register(Ref<AllocatorData> allocatorData);
		void UnRegister(Ref<AllocatorData> allocatorData);

		inline void UpdateTotalSize(Size size) { m_TotalSizeAllocated += size; }

		Size GetUsedAllocatedSize() const;
		inline Size GetTotalSizeAllocated() const { return m_TotalSizeAllocated; }
		inline Size GetApplicationMemoryBudget() const { return m_ApplicationBudget; }

	  private:
		std::vector<Ref<AllocatorData>> allocators;
		Size m_ApplicationBudget;
		Size m_TotalSizeAllocated;
	};
} // namespace QMBT
