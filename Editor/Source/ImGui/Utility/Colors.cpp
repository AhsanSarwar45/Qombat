#include "Colors.hpp"

#include <Qombat/Utility.hpp>

namespace QCreate
{

	RandomColors::RandomColors(int length)
		: m_Length(length)
	{
		for (size_t i = 0; i < m_Length; i++)
		{
			m_Colors.push_back(ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
		}
	}

	ImColor RandomColors::GenerateRandomColor()
	{
		return ImColor(QMBT::Utility::RandRangeFloat(0, 1),
					   QMBT::Utility::RandRangeFloat(0, 1),
					   QMBT::Utility::RandRangeFloat(0, 1));
	}
} // namespace QCreate