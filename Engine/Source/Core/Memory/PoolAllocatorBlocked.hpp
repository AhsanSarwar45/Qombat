#pragma once

#include <QMBTPCH.hpp>

#include "Core/Collections.hpp"

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
	class PoolAllocatorBlocked
	{
	  public:
		//Prohibit default construction, moving and assignment
		// ! For some reason, the default constructor does not get deleted

		PoolAllocatorBlocked(const PoolAllocatorBlocked&) = delete;
		PoolAllocatorBlocked(PoolAllocatorBlocked&&) = delete;
		PoolAllocatorBlocked& operator=(const PoolAllocatorBlocked&) = delete;
		PoolAllocatorBlocked& operator=(PoolAllocatorBlocked&&) = delete;

		/**
		 * @brief Construct a new Pool Allocator object.
		 * 
		 * @param debugName The name that will appear in logs and any editor.
		 * @param chunksPerBlock After this many items have been allocated, the allocator allocates
		 * a new block of size equal to chunksPerBlock * sizeof(Object). 
		 */
		PoolAllocatorBlocked(const char* debugName = "Allocator", Size chunksPerBlock = 8);
		~PoolAllocatorBlocked();

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
		PoolAllocatorBlocked();
		PoolAllocatorBlocked(PoolAllocatorBlocked&);
		Chunk* AllocateBlock(Size chunkSize);

	  private:
		Ref<AllocatorData> m_Data;

		Size m_ChunksPerBlock;
		Size m_ObjectSize;

		Chunk* m_CurrentPtr = nullptr;
		BasicVector<Chunk*> m_AllocatedBlocks;
	};

	template <typename Object>
	PoolAllocatorBlocked<Object>::PoolAllocatorBlocked(const char* debugName, Size chunksPerBlock)
		: m_ChunksPerBlock(chunksPerBlock)
	{
		QMBT_CORE_ASSERT(m_ChunksPerBlock > 0, "Chunks per block have to be more than 0!");

		m_ObjectSize = sizeof(Object);

		m_Data = std::make_shared<AllocatorData>(debugName, 0);

		MemoryManager::GetInstance()
			.Register(m_Data);

		m_CurrentPtr = AllocateBlock(m_ObjectSize);
		m_AllocatedBlocks.push_back(m_CurrentPtr);
	}

	template <typename Object>
	PoolAllocatorBlocked<Object>::~PoolAllocatorBlocked()
	{
		MemoryManager::GetInstance().UnRegister(m_Data);
		for (auto& ptr : m_AllocatedBlocks)
		{
			free(ptr);
		}
	}

	template <typename Object>
	void* PoolAllocatorBlocked<Object>::Allocate()
	{
		// No chunks left in the current block, or no block
		// exists yet, allocate a new one.
		if (m_CurrentPtr == nullptr)
		{
			m_CurrentPtr = AllocateBlock(m_ObjectSize);
			m_AllocatedBlocks.push_back(m_CurrentPtr);
		}

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
	void PoolAllocatorBlocked<Object>::Deallocate(Object* ptr)
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
	void PoolAllocatorBlocked<Object>::Delete(Object* ptr)
	{
		ptr->~Object();	 // Call the destructor on the object
		Deallocate(ptr); // Deallocate the pointer
	}

	template <typename Object>
	Chunk* PoolAllocatorBlocked<Object>::AllocateBlock(Size chunkSize)
	{
		QMBT_CORE_ASSERT(chunkSize > sizeof(Chunk), "Object size must be larger than pointer size");

		// The total memory (in Bytes), to be allocated
		Size blockSize = m_ChunksPerBlock * chunkSize;

		// The first chunk of the new block
		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

		m_Data->TotalSize += blockSize;
		MemoryManager::GetInstance().UpdateTotalSize(blockSize);

		// Once the block is allocated, we need to chain all
		// the chunks in this block:

		Chunk* chunk = blockBegin;

		for (int i = 0; i < m_ChunksPerBlock - 1; ++i)
		{
			chunk->next =
				reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
			chunk = chunk->next;
		}

		chunk->next = nullptr;

		LOG_CORE_INFO("{0} Allocated block ({1} chunks)", m_Data->DebugName, m_ChunksPerBlock);

		return blockBegin;
	}

} // namespace QMBT