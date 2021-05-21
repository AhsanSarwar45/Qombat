#include <vector>

#include <ImGui/imgui.h>

#include <Qombat/Core.hpp>
#include <Qombat/Events.hpp>

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
		void OnEvent(Event& event);

	  private:
		void SetFrameData();
		bool OnMouseScroll(MouseScrolledEvent& event);

		void DrawCPUProfiler();
		void DrawMemoryProfiler();

	  private:
		UInt16 m_BarHeight {30};
		RandomColors m_Colors {10};
		Vector<Frame>* m_Frames {nullptr};
		Frame* m_SelectedFrame {nullptr};
		double m_SelectedFrameTime {0};
		double m_FrameMarkerPos {0};
		float m_FrameAnalyserZoom {200};
	};
} // namespace QCreate