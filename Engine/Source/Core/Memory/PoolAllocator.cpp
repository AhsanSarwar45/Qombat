// #include "PoolAllocator.hpp"

// namespace QMBT
// {
// 	template <typename Object>
// 	PoolAllocator<Object>::PoolAllocator(const std::string& debugName, Size chunksPerBlock)
// 		: m_ChunksPerBlock(chunksPerBlock)
// 	{
// 		QMBT_CORE_ASSERT(m_ChunksPerBlock > 0, "Chunks per block have to be more than 0!");

// 		m_ObjectSize = sizeof(Object);
// 		m_Data = std::make_shared<AllocatorData>(debugName, 0);

// 		MemoryManager::GetInstance().Register(m_Data);
// 	}

// 	template <typename Object>
// 	PoolAllocator<Object>::~PoolAllocator()
// 	{
// 		MemoryManager::GetInstance().UnRegister(m_Data);
// 		free(m_HeadPtr);
// 	}

// 	template <typename Object>
// 	Object* PoolAllocator<Object>::Allocate()
// 	{

// 		// No chunks left in the current block, or no any block
// 		// exists yet. Allocate a new one, passing the chunk size:

// 		if (m_HeadPtr == nullptr)
// 		{
// 			m_HeadPtr = AllocateBlock(m_ObjectSize);
// 		}

// 		// The return value is the current position of
// 		// the allocation pointer:

// 		Chunk* freeChunk = m_HeadPtr;

// 		// Advance (bump) the allocation pointer to the next chunk.
// 		//
// 		// When no chunks left, the `mAlloc` will be set to `nullptr`, and
// 		// this will cause allocation of a new block on the next request:

// 		m_HeadPtr = m_HeadPtr->next;

// 		m_Data->UsedSize += m_ObjectSize;
// 		LOG_CORE_INFO("{0} Allocated {1} bytes", m_Data->DebugName, m_ObjectSize);

// 		return reinterpret_cast<Object*>(freeChunk);
// 	}

// 	/**
//  * Allocates a new block from OS.
//  *
//  * Returns a Chunk pointer set to the beginning of the block.
//  */
// 	template <typename Object>
// 	Chunk* PoolAllocator<Object>::AllocateBlock(Size chunkSize)
// 	{
// 		QMBT_CORE_ASSERT(chunkSize > sizeof(Chunk), "Object size must be larger than pointer size");
// 		Size blockSize = m_ChunksPerBlock * chunkSize;

// 		// The first chunk of the new block.
// 		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

// 		m_Data->TotalSize += blockSize;
// 		MemoryManager::GetInstance().UpdateTotalSize(blockSize);

// 		// Once the block is allocated, we need to chain all
// 		// the chunks in this block:

// 		Chunk* chunk = blockBegin;

// 		for (int i = 0; i < m_ChunksPerBlock - 1; ++i)
// 		{
// 			chunk->next =
// 				reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
// 			chunk = chunk->next;
// 		}

// 		chunk->next = nullptr;

// 		LOG_CORE_INFO("{0} Allocated block ({1} chunks)", m_Data->DebugName, m_ChunksPerBlock);

// 		return blockBegin;
// 	}

// 	/**
//  * Puts the chunk into the front of the chunks list.
//  */
// 	template <typename Object>
// 	void PoolAllocator<Object>::Deallocate(Object* chunk)
// 	{

// 		// The freed chunk's next pointer points to the
// 		// current allocation pointer:

// 		reinterpret_cast<Chunk*>(chunk)->next = m_HeadPtr;

// 		// And the allocation pointer is now set
// 		// to the returned (free) chunk:

// 		m_HeadPtr = reinterpret_cast<Chunk*>(chunk);

// 		m_Data->UsedSize -= m_ObjectSize;
// 		LOG_CORE_INFO("{0} Deallocated {1} bytes", m_Data->DebugName, m_ObjectSize);
// 	}
// } // namespace QMBT
