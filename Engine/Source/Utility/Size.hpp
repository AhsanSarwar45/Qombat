#pragma once
#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Utility/Strings.hpp"

namespace QMBT
{
	namespace Utility
	{
		constexpr inline float ToKB(Size x)
		{
			return x / 1024.0f;
		}

		constexpr inline float ToMB(Size x)
		{
			return x / 1048576.0f;
		}

		constexpr inline float ToGB(Size x)
		{
			return x / 1073741824.0f;
		}

		inline std::string ToReadable(Size x)
		{
			std::stringstream stream;
			if (x < 1024)
			{
				stream << std::fixed << std::setprecision(2) << x;
				return Concatenate(stream.str(), " B");
			}
			else if (x < 1048576)
			{
				stream << std::fixed << std::setprecision(2) << x / 1024.0f;
				return Concatenate(stream.str(), " KB");
			}
			else if (x < 1073741824)
			{
				stream << std::fixed << std::setprecision(2) << x / 1048576.0f;
				return Concatenate(stream.str(), " MB");
			}
			else
			{
				stream << std::fixed << std::setprecision(2) << x / 1073741824.0f;
				return Concatenate(stream.str(), " GB");
			}
		}
	} // namespace Utility
} // namespace QMBT
