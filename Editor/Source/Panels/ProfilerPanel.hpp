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
		UInt16 m_BarHeight;
		RandomColors m_Colors;
		Vector<Frame>* m_Frames;
		Frame* m_SelectedFrame;
		double m_SelectedFrameTime;
		double m_FrameMarkerPos;
		float m_FrameAnalyserZoom = 200;
	};
} // namespace QCreate