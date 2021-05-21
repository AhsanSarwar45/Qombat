#pragma once
#include <random>
#include <vector>

#include <ImGui/imgui.h>

namespace QCreate
{
	class RandomColors
	{
	  public:
		explicit RandomColors(int length);

		inline ImColor GetRandomColor() { return m_Colors[m_CurrentIndex++]; }
		inline void Reset() { m_CurrentIndex = 0; }

	  private:
		ImColor GenerateRandomColor();

		std::vector<ImColor> m_Colors;
		int m_Length;
		int m_CurrentIndex{0};
	};
} // namespace QCreate