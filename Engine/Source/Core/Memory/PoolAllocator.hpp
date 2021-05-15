#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/MemoryManager.hpp"

namespace QMBT
{
	/**
	 * @brief A templated allocator that can only be used to allocate memory for a 
	 * collection of the same type. 
	 * 
	 * @tparam Object 
	 */
	template <typename Object>
	class PoolAllocator
	{
	  public:
		//Prohibit default construction, moving and assignment
		PoolAllocator(const PoolAllocator&) = delete;
		PoolAllocator(PoolAllocator&&) = delete;
		PoolAllocator& operator=(const PoolAllocator&) = delete;
		PoolAllocator& operator=(PoolAllocator&&) = delete;

		/**
		 * @brief Construct a new Pool Allocator object.
		 * 
		 * @param debugName The name that will appear in logs and any editor.
		 * @param chunksPerBlock After this many items have been allocated, the allocator allocates
		 * a new block of size equal to chunksPerBlock * sizeof(Object). 
		 */
		PoolAllocator(const char* debugName = "Allocator", Size numObjects = 1);
		~PoolAllocator();

		/**
		 * @brief Gets an address in the pool, constructs the object at the address and returns the address
		 * 
		 * @return Object* The pointer to the newly allocated memory
		 */
		void* Allocate();

		template <typename... Args>
		Object* New(Args... argList)
		{
			void* address = Allocate();				 // Allocate the raw memory and get a pointer to it
			return new (address) Object(argList...); //Call the placement new operator, which constructs the Object
		}

		void Deallocate(Object* ptr);
		void Delete(Object* ptr);

		inline Size GetUsedSize() const { return m_Data->UsedSize; }

	  private:
		PoolAllocator(PoolAllocator&);
		Chunk* AllocateBlock(Size chunkSize);

	  private:
		Ref<AllocatorData> m_Data;

		Size m_NumObjects;
		Size m_ObjectSize;

		Chunk* m_HeadPtr = nullptr;
		Chunk* m_CurrentPtr = nullptr;
	};

	template <typename Object>
	PoolAllocator<Object>::PoolAllocator(const char* debugName, Size numObjects)
		: m_NumObjects(numObjects)
	{
		QMBT_CORE_ASSERT(numObjects > 0, "Number of objects have to be more than 0!");

		m_ObjectSize = sizeof(Object);

		m_Data = std::make_shared<AllocatorData>(debugName, 0);

		MemoryManager::GetInstance()
			.Register(m_Data);

		m_HeadPtr = AllocateBlock(m_ObjectSize);
		m_CurrentPtr = m_HeadPtr;
	}

	template <typename Object>
	PoolAllocator<Object>::~PoolAllocator()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
		free(m_HeadPtr);
	}

	template <typename Object>
	void* PoolAllocator<Object>::Allocate()
	{

		QMBT_CORE_ASSERT(m_CurrentPtr, "Allocator out of memory!");

		// The return value is the current position of
		// the allocation pointer:
		Chunk* freeChunk = m_CurrentPtr;

		// Advance (bump) the allocation pointer to the next chunk.
		// When no chunks left, the `m_CurrentPtr` will be set to `nullptr`, and
		// this will cause allocation of a new block on the next request:
		m_CurrentPtr = m_CurrentPtr->next;

		m_Data->UsedSize += m_ObjectSize;
		LOG_CORE_INFO("{0} Allocated {1} bytes", m_Data->DebugName, m_ObjectSize);

		return freeChunk;
	}
	template <typename Object>
	void PoolAllocator<Object>::Deallocate(Object* ptr)
	{
		// The freed chunk's next pointer points to the
		// current allocation pointer:
		reinterpret_cast<Chunk*>(ptr)->next = m_CurrentPtr;

		// And the allocation pointer is now set
		// to the returned (free) chunk:

		m_CurrentPtr = reinterpret_cast<Chunk*>(ptr);

		m_Data->UsedSize -= m_ObjectSize;
		LOG_CORE_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, m_ObjectSize);
	}

	template <typename Object>
	void PoolAllocator<Object>::Delete(Object* ptr)
	{
		ptr->~Object();	 // Call the destructor on the object
		Deallocate(ptr); // Deallocate the pointer
	}

	template <typename Object>
	Chunk* PoolAllocator<Object>::AllocateBlock(Size chunkSize)
	{
		QMBT_CORE_ASSERT(chunkSize > sizeof(Chunk), "Object size must be larger than pointer size");

		// The total memory (in Bytes), to be allocated
		Size blockSize = m_NumObjects * chunkSize;

		// The first chunk of the new block
		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

		m_Data->TotalSize += blockSize;
		MemoryManager::GetInstance().UpdateTotalSize(blockSize);

		// Once the block is allocated, we need to chain all
		// the chunks in this block:

		Chunk* chunk = blockBegin;

		for (int i = 0; i < m_NumObjects - 1; ++i)
		{
			chunk->next =
				reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
			chunk = chunk->next;
		}

		chunk->next = nullptr;

		LOG_CORE_INFO("{0} Allocated block ({1} chunks)", m_Data->DebugName, m_NumObjects);

		return blockBegin;
	}

} // namespace QMBT