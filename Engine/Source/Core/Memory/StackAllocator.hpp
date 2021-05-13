#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/MemoryManager.hpp"
#include "Core/Memory/Utility/MemoryUtils.hpp"

namespace QMBT
{

	/**
	 * @brief A custom memory allocator which allocates in a stack-like manner
	 * @details All the memory will be allocated up-front. This means we will have
	 * zero allocations during runtime. This also means that it will take the same
	 * amount of RAM memory whether it is full or empty. Allocations and Deallocations
	 * also need to be done in a stack-like manner. It is the task of the user to 
	 * make sure that deallocations happen in an order that is the reverse of the allocation
	 * order.
	 */
	class StackAllocator
	{
	  public:
		//Prohibit default construction, moving and assignment
		StackAllocator() = delete;
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;
		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator&&) = delete;

		/**
		 * @brief Construct a new Stack Allocator object.
		 * 
		 * @param debugName The name that will appear in logs and any editor.
		 * @param totalSize This will be allocated up-front. Even if the stack allocator is empty, it will
		 * consume this amount of memory.
		 */
		StackAllocator(const std::string& debugName = "Allocator", const Size totalSize = 50_MB);

		~StackAllocator();

		/**
		 * @brief Allocates raw memory without calling any constructor
		 * @details Allocation complexity is O(1)
		 * 
		 * @param size The size of the memory to be allocated in bytes
		 * @param alignment The alignment of the memory to be allocated in bytes
		 * @return void* The pointer to the newly allocated memory
		 */
		void* Allocate(const Size size, const Size alignment = 8);

		/**
		 * @brief Allocates a new block of memory and calls the constructor
		 * @details Allocation complexity is O(1)
		 * 
		 * @tparam Object The type to be created
		 * @tparam Args Variadic arguments
		 * @param argList The arguments to the constructor of the type Object
		 * @return Object* The pointer to the newly allocated and created object
		 */
		template <typename Object, typename... Args>
		Object* New(Args... argList);

		/**
		 * @brief Deallocates raw memory without calling any destructor
		 * @details Deallocation complexity is O(1)
		 * 
		 * @param ptr The pointer to the memory to be deallocated
		 */
		void Deallocate(const Size ptr);

		/**
		 * @brief Deallocates a pointer and calls the destructor
		 * @details Deallocation complexity is O(1)
		 * 
		 * @tparam Object The type of the passed pointer
		 * @param ptr The pointer to the memory to be deallocated
		 */
		template <typename Object>
		void Delete(Object* ptr);

		inline Size GetUsedSpace() const { return m_Data->UsedSize; }

	  private:
		StackAllocator(StackAllocator& stackAllocator); //Restrict copying

		Ref<AllocatorData> m_Data;

		void* m_HeadPtr = nullptr; // Points to the first available location
		Size m_Offset;

		struct AllocationHeader
		{
			char padding;
		};
	};

	template <typename Object, typename... Args>
	Object* StackAllocator::New(Args... argList)
	{
		void* address = Allocate(sizeof(Object)); // Allocate the raw memory and get a pointer to it
		return new (address) Object(argList...);  //Call the placement new operator, which constructs the Object
	}

	template <typename Object>
	void StackAllocator::Delete(Object* ptr)
	{
		ptr->~Object();		   // Call the destructor on the object
		Deallocate(Size(ptr)); // Deallocate the pointer
	}

} // namespace QMBT