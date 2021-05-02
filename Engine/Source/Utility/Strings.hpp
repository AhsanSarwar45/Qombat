#pragma once
#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"

namespace QMBT
{
	namespace Utility
	{
		namespace Detail
		{
			template <typename>
			struct StringSizeImpl;

			template <Size N>
			struct StringSizeImpl<const char[N]>
			{
				static constexpr Size GetSize(const char (&)[N]) { return N - 1; }
			};

			template <Size N>
			struct StringSizeImpl<char[N]>
			{
				static Size GetSize(char (&s)[N]) { return N ? strlen(s) : 0; }
			};

			template <>
			struct StringSizeImpl<const char*>
			{
				static Size GetSize(const char* s) { return s ? strlen(s) : 0; }
			};

			template <>
			struct StringSizeImpl<char*>
			{
				static Size GetSize(char* s) { return s ? strlen(s) : 0; }
			};

			template <>
			struct StringSizeImpl<std::string>
			{
				static Size GetSize(const std::string& s) { return s.size(); }
			};

			template <typename String>
			Size string_size(String&& s)
			{
				using noref_t = typename std::remove_reference<String>::type;
				using string_t = typename std::conditional<std::is_array<noref_t>::value,
														   noref_t,
														   typename std::remove_cv<noref_t>::type>::type;
				return StringSizeImpl<string_t>::GetSize(s);
			}

			template <typename...>
			struct ConcatenateImpl;

			template <typename String>
			struct ConcatenateImpl<String>
			{
				static Size GetSize(String&& s) { return string_size(s); }
				static void Concatenate(std::string& result, String&& s) { result += s; }
			};

			template <typename String, typename... Rest>
			struct ConcatenateImpl<String, Rest...>
			{
				static Size GetSize(String&& s, Rest&&... rest)
				{
					return string_size(s) + ConcatenateImpl<Rest...>::GetSize(std::forward<Rest>(rest)...);
				}
				static void Concatenate(std::string& result, String&& s, Rest&&... rest)
				{
					result += s;
					ConcatenateImpl<Rest...>::Concatenate(result, std::forward<Rest>(rest)...);
				}
			};
		} // namespace Detail

		template <typename... Strings>
		std::string Concatenate(Strings&&... strings)
		{
			std::string result;
			result.reserve(Detail::ConcatenateImpl<Strings...>::GetSize(std::forward<Strings>(strings)...));
			Detail::ConcatenateImpl<Strings...>::Concatenate(result, std::forward<Strings>(strings)...);
			return result;
		}
	} // namespace Utility

} // namespace QMBT