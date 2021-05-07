#include <vector>

#include <Qombat/Core.hpp>
#include <imgui/imgui.h>

#include "ImGui/Utility/Colors.hpp"

namespace QCreate
{
	using namespace QMBT;

	class ProfilerPanel
	{
	  public:
		ProfilerPanel();
		~ProfilerPanel();

		void Draw();

	  private:
		UInt16 m_BarHeight;
		RandomColors m_Colors;
		std::vector<Frame>* m_Frames;
	};
} // namespace QCreate