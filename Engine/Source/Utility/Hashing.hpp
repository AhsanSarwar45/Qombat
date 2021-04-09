// The MIT License(MIT)
//
// Copyright(c) 2015 Stefan Reinalter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"

namespace QMBT
{

	namespace HashCompileTime
	{
		template <Size I>
		struct Hash
		{
			// Recursive method for generating hash at compile time
			template <Size Length>
			constexpr static inline UInt32 Generate(const char (&str)[Length])
			{
				return static_cast<UInt32>(static_cast<UInt64>(Hash<I - 1u>::Generate(str) ^ UInt32(str[I - 1u])) * 16777619ull);
			}
		};

		// Specialization for string of length 0
		template <>
		struct Hash<0u>
		{
			template <Size Length>
			constexpr static inline UInt32 Generate(const char (&str)[Length])
			{
				return 2166136261u;
			}
		};
	} // namespace HashCompileTime

	namespace HashRunTime
	{
		// Iterative method for generating hash at run time
		inline UInt32 Fnv1aHash(UInt32 hash, const char* str, size_t length)
		{
			for (Size i = 0; i < length; ++i)
			{
				const UInt32 value = static_cast<uint32_t>(*str++);
				hash ^= value;
				hash *= 16777619u;
			}

			return hash;
		}

		inline UInt32 Fnv1aHash(const char* str, Size length)
		{
			return Fnv1aHash(2166136261u, str, length);
		}

		inline UInt32 Fnv1aHash(const char* str)
		{
			return Fnv1aHash(str, strlen(str));
		}
	} // namespace HashRunTime

	namespace HashDetail
	{
		template <typename T>
		struct HashHelper
		{
		};

		template <>
		struct HashHelper<const char*>
		{
			static inline UInt32 Generate(const char* str)
			{
				return HashRunTime::Fnv1aHash(str);
			}
		};

		template <Size Length>
		struct HashHelper<char[Length]>
		{
			constexpr static inline UInt32 Generate(const char (&str)[Length])
			{
				return HashCompileTime::Hash<Length - 1u>::Generate(str);
			}
		};
	} // namespace HashDetail

	template <typename T>
	constexpr static inline UInt32 GenerateHash(const T& str)
	{
		return HashDetail::HashHelper<T>::Generate(str);
	}

	class StringHash
	{
	  public:
		template <typename T>
		StringHash(const T& str)
			: m_Hash(GenerateHash(str))
		{
		}

		inline UInt32 Get(void) const
		{
			return m_Hash;
		}

		friend inline std::ostream& operator<<(std::ostream& os, const StringHash& hash);

	  private:
		const UInt32 m_Hash;
	};

	inline std::ostream& operator<<(std::ostream& os, const StringHash& hash)
	{
		return os << hash.m_Hash;
	}

} // namespace QMBT
