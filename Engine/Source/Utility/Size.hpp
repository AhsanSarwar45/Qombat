#pragma once
#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Utility/Strings.hpp"

namespace QMBT
{
	namespace Utility
	{
		constexpr inline Size ToKB(Size x)
		{
			return x / 1024;
		}

		constexpr inline Size ToMB(Size x)
		{
			return x / 1048576;
		}

		constexpr inline Size ToGB(Size x)
		{
			return x / 1073741824;
		}

		inline std::string ToReadable(Size x)
		{
			if (x < 1024)
			{
				return Concatenate(std::to_string(x), " B");
			}
			else if (x < 1048576)
			{
				return Concatenate(std::to_string(x / 1024), " KB");
			}
			else if (x < 1073741824)
			{
				return Concatenate(std::to_string(x / 1048576), " MB");
			}
			else
			{
				return Concatenate(std::to_string(x / 1073741824), " GB");
			}
		}
	} // namespace Utility
} // namespace QMBT
