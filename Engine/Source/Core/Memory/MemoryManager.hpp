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

	using AllocatorVector = std::vector<Ref<AllocatorData>>;

	class MemoryManager
	{
	  public:
		MemoryManager(Size applicationBudget)
			: m_ApplicationBudget(applicationBudget), m_TotalAllocatedSize(0) {}
		~MemoryManager();

		static MemoryManager& GetInstance();

		void Register(Ref<AllocatorData> allocatorData);
		void UnRegister(Ref<AllocatorData> allocatorData);

		inline void UpdateTotalSize(Size size) { m_TotalAllocatedSize += size; }

		Size GetUsedAllocatedSize() const;
		inline Size GetTotalAllocatedSize() const { return m_TotalAllocatedSize; }
		inline Size GetApplicationMemoryBudget() const { return m_ApplicationBudget; }
		inline const AllocatorVector& GetAllocators() const { return allocators; }

	  private:
		AllocatorVector allocators;
		Size m_ApplicationBudget;
		Size m_TotalAllocatedSize;
	};
} // namespace QMBT
