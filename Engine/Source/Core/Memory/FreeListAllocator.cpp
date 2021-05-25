#include "FreeListAllocator.hpp"

#include "Core/Core.hpp"
#include "MemoryManager.hpp"
#include "Utility/MemoryUtils.hpp"

namespace QMBT
{
	FreeListAllocator::FreeListAllocator(const char* debugName, const Size totalSize, const PlacementPolicy policy)
		: m_Policy(policy), m_Data(std::make_shared<AllocatorData>(debugName, totalSize))
	{
		// Allows the memory manager to keep track of total allocated memory
		MemoryManager::GetInstance().Register(m_Data);

		Init();
	}

	void FreeListAllocator::Init()
	{
		if (m_StartPtr != nullptr)
		{
			free(m_StartPtr);
			m_StartPtr = nullptr;
		}
		m_StartPtr = malloc(m_Data->TotalSize);

		this->Reset();
	}

	FreeListAllocator::~FreeListAllocator()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
		free(m_StartPtr);
	}

	void* FreeListAllocator::Allocate(const Size size, const Size alignment, const char* name)
	{
		//QMBT_CORE_ASSERT(size >= sizeof(Node), "Allocation size must be bigger than size of Node");
		QMBT_CORE_ASSERT(alignment >= 8, "Alignment must be 8 at least");

		static Size allocationHeaderSize = sizeof(FreeListAllocator::AllocationHeader);
		static Size freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);

		// Search through the free list for a free block that has enough space to allocate our data
		Size padding;
		Node* affectedNode;
		Node* previousNode;
		this->Find(size, alignment, padding, previousNode, affectedNode);
		QMBT_CORE_ASSERT(affectedNode != nullptr, "Not enough memory");

		const Size alignmentPadding = padding - allocationHeaderSize;
		const Size requiredSize = size + padding;

		const Size rest = affectedNode->data.blockSize - requiredSize;

		if (rest > 0)
		{
			// We have to split the block into the data block and a free block of size 'rest'
			Node* newFreeNode = (Node*)((Size)affectedNode + requiredSize);
			newFreeNode->data.blockSize = rest;
			m_FreeList.Insert(affectedNode, newFreeNode);
		}
		m_FreeList.Remove(previousNode, affectedNode);

		// Setup data block
		const Size headerAddress = (Size)affectedNode + alignmentPadding;
		const Size dataAddress = headerAddress + allocationHeaderSize;
		((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize = requiredSize;
		((FreeListAllocator::AllocationHeader*)headerAddress)->padding = alignmentPadding;

		m_Data->UsedSize += requiredSize;

		if (*name != 0)
		{
			m_Data->Allocations[name] += requiredSize;
		}

		return (void*)dataAddress;
	}

	void FreeListAllocator::Find(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode)
	{
		switch (m_Policy)
		{
		case FIND_FIRST:
			FindFirst(size, alignment, padding, previousNode, foundNode);
			break;
		case FIND_BEST:
			FindBest(size, alignment, padding, previousNode, foundNode);
			break;
		}
	}

	void FreeListAllocator::FindFirst(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode)
	{
		//Iterate list and return the first free block with a size >= than given size
		Node* it = m_FreeList.head;
		Node* itPrev = nullptr;

		while (it != nullptr)
		{
			padding = Utility::CalculatePaddingWithHeader((Size)it, alignment, sizeof(FreeListAllocator::AllocationHeader));
			const Size requiredSpace = size + padding;
			if (it->data.blockSize >= requiredSpace)
			{
				break;
			}
			itPrev = it;
			it = it->next;
		}
		previousNode = itPrev;
		foundNode = it;
	}

	void FreeListAllocator::FindBest(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode)
	{
		// Iterate WHOLE list keeping a pointer to the best fit
		Size smallestDiff = std::numeric_limits<Size>::max();
		Node* bestBlock = nullptr;
		Node *it = m_FreeList.head,
			 *itPrev = nullptr;
		while (it != nullptr)
		{
			padding = Utility::CalculatePaddingWithHeader((Size)it, alignment, sizeof(FreeListAllocator::AllocationHeader));
			const Size requiredSpace = size + padding;
			if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
			{
				bestBlock = it;
			}
			itPrev = it;
			it = it->next;
		}
		previousNode = itPrev;
		foundNode = bestBlock;
	}

	void FreeListAllocator::Deallocate(void* ptr, const char* name)
	{
		// Insert it in a sorted position by the address number
		const Size headerAddress = (Size)ptr - sizeof(FreeListAllocator::AllocationHeader);
		const FreeListAllocator::AllocationHeader* allocationHeader{(FreeListAllocator::AllocationHeader*)headerAddress};

		Node* freeNode = (Node*)(headerAddress);
		freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
		freeNode->next = nullptr;

		Node* it = m_FreeList.head;
		Node* itPrev = nullptr;
		while (it != nullptr)
		{
			if (ptr < it)
			{
				m_FreeList.Insert(itPrev, freeNode);
				break;
			}
			itPrev = it;
			it = it->next;
		}

		m_Data->UsedSize -= freeNode->data.blockSize;

		if (*name != 0)
		{
			m_Data->Allocations[name] -= freeNode->data.blockSize;
		}
		// Merge contiguous nodes
		Coalescence(itPrev, freeNode);
	}

	void FreeListAllocator::Coalescence(Node* previousNode, Node* freeNode)
	{
		if (freeNode->next != nullptr &&
			(Size)freeNode + freeNode->data.blockSize == (Size)freeNode->next)
		{
			freeNode->data.blockSize += freeNode->next->data.blockSize;
			m_FreeList.Remove(freeNode, freeNode->next);
		}

		if (previousNode != nullptr &&
			(Size)previousNode + previousNode->data.blockSize == (Size)freeNode)
		{
			previousNode->data.blockSize += freeNode->data.blockSize;
			m_FreeList.Remove(previousNode, freeNode);
		}
	}

	void FreeListAllocator::Reset()
	{
		m_Data->UsedSize = 0;
		Node* firstNode = (Node*)m_StartPtr;
		firstNode->data.blockSize = m_Data->TotalSize;
		firstNode->next = nullptr;
		m_FreeList.head = nullptr;
		m_FreeList.Insert(nullptr, firstNode);
	}

} // namespace QMBT