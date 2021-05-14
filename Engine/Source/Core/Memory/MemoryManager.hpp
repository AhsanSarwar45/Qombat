#pragma once

#include <QMBTPCH.hpp>

#include "AllocatorData.hpp"
#include "Core/Core.hpp"

namespace QMBT
{
	class MemoryManager
	{
	  public:
		MemoryManager(Size applicationBudget);
		~MemoryManager();

		static MemoryManager& GetInstance();

		void Register(Ref<AllocatorData> allocatorData);
		void UnRegister(Ref<AllocatorData> allocatorData);

		inline void UpdateTotalSize(Size size) { m_TotalAllocatedSize += size; }

		Size GetUsedAllocatedSize() const;
		inline Size GetTotalAllocatedSize() const { return m_TotalAllocatedSize; }
		inline Size GetApplicationMemoryBudget() const { return m_ApplicationBudget; }
		inline const AllocatorVector& GetAllocators() const { return m_Allocators; }
		//inline static const VectorAllocator& GetVectorAllocator() {return GetInstance().m_VectorAllocator;}

	  private:
		AllocatorVector m_Allocators;
		//AllocatorVector vectorAllocators;

		//VectorAllocator m_VectorAllocator;

		Size m_ApplicationBudget;
		Size m_TotalAllocatedSize;
	};
} // namespace QMBT
