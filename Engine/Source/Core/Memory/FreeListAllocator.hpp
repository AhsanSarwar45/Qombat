#pragma once

#include <QMBTPCH.hpp>

#include "AllocatorData.hpp"
#include "Core/Aliases.hpp"
#include "Core/Types/SinglyLinkedList.hpp"

namespace QMBT
{
	class FreeListAllocator
	{
	  public:
		enum PlacementPolicy
		{
			FIND_FIRST, // For better speed, finds the first block of memory of sufficeint size.
			FIND_BEST	// For better memory packing, finds all the blocks of memory of sufficient size and chooses the
						// one with the tightest fit.
		};

	  private:
		struct FreeHeader
		{
			Size blockSize;
		};
		struct AllocationHeader
		{
			Size blockSize;
			char padding;
		};

		using Node = SinglyLinkedList<FreeHeader>::Node;

	  public:
		FreeListAllocator(const char* debugName = "FreeListAllocator", const Size totalSize = 50_MB, const PlacementPolicy policy = PlacementPolicy::FIND_FIRST);

		~FreeListAllocator();

		void* Allocate(const Size size, const Size alignment = 8, const char* name = "");

		template <typename Object, typename... Args>
		Object* New(Args... argList);

		void Deallocate(void* ptr, const char* name = "");

		template <typename Object>
		void Delete(Object* ptr);

		void Init();

		void Reset();

		inline Size GetUsedSize() const { return m_Data->UsedSize; }

	  private:
		FreeListAllocator(FreeListAllocator& freeListAllocator);

		void Coalescence(Node* prevBlock, Node* freeBlock);

		void Find(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode);
		void FindBest(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode);
		void FindFirst(const Size size, const Size alignment, Size& padding, Node*& previousNode, Node*& foundNode);

	  private:
		void* m_StartPtr = nullptr;
		PlacementPolicy m_Policy;
		SinglyLinkedList<FreeHeader> m_FreeList;
		std::shared_ptr<AllocatorData> m_Data;
	};

	template <typename Object, typename... Args>
	Object* FreeListAllocator::New(Args... argList)
	{
		void* address = Allocate(sizeof(Object)); // Allocate the raw memory and get a pointer to it
		return new (address) Object(argList...);  //Call the placement new operator, which constructs the Object
	}

	template <typename Object>
	void FreeListAllocator::Delete(Object* ptr)
	{
		ptr->~Object();	 // Call the destructor on the object
		Deallocate(ptr); // Deallocate the pointer
	}
} // namespace QMBT
