#pragma once

namespace Qombat
{

	template <typename T>
	class Vector
	{
	  public:
		typedef T Type;
		typedef T* Pointer;
		typedef const T* ConstPointer;
		typedef T& Reference;
		typedef const T& ConstReference;
		typedef T* Iterator;
		typedef const T* ConstIterator;
	};
} // namespace Qombat