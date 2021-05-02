#pragma once
#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"

namespace QMBT
{
	namespace Utility
	{
		inline float RandRangeFloat(float min, float max)
		{
			static std::default_random_engine generator;
			static std::uniform_real_distribution<> distributor(0, 1); // rage 0 - 1

			return distributor(generator);
		}

	} // namespace Utility
} // namespace QMBT