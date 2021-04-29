#pragma once

#include <QMBTPCH.hpp>

#include "Core/Memory/MemoryManager.hpp"
#include "Core/Memory/Utility/MemoryUtils.hpp"

namespace QMBT
{

	/**
	 * @brief A custom memory allocator which allocates in a stack-like manner
	 * @details 
	 */
	class StackAllocator
	{
	  public:
		StackAllocator() = delete;
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;

		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator&&) = delete;

		StackAllocator(const std::string& debugName = "Allocator", const Size totalSize = 50_MB);

		~StackAllocator();

		/**
		 * @brief Allocates raw memory without calling any constructor
		 * 
		 * @param size The size of the memory to be allocated in bytes
		 * @param alignment The alignment of the memory to be allocated in bytes
		 * @return void* The pointer to the newly allocated memory
		 */
		void* Allocate(const Size size, const Size alignment = 8);

		/**
		 * @brief Allocates a new block of memory and calls the constructor on of the Object
		 * 
		 * @tparam Object The type to be created
		 * @tparam Args Variadic arguments
		 * @param argList The arguments to the constructor of the type Object
		 * @return Object* The pointer to the newly allocated and created object
		 */
		template <typename Object, typename... Args>
		Object* New(Args... argList);

		void Deallocate(const Size ptr);

		template <typename Object>
		void Delete(Object* ptr);

		void Clear();

	  private:
		StackAllocator(StackAllocator& stackAllocator); //Restrict copying

		Ref<AllocatorData> m_Data;

		void* m_HeadPtr = nullptr;
		Size m_Offset;

		struct AllocationHeader
		{
			char padding;
		};
	};

	template <typename Object, typename... Args>
	Object* StackAllocator::New(Args... argList)
	{
		void* address = Allocate(sizeof(Object));
		return new (address) Object(argList...); //Call the placement new operator, which constructs the Object
	}

	template <typename Object>
	void StackAllocator::Delete(Object* ptr)
	{
		Deallocate(Size(ptr));
		ptr->~Object(); 
	}

} // namespace QMBT