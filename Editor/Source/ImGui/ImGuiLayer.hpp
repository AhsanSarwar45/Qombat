#pragma once

#include <Qombat/Core.hpp>

#include "Panels/ProfilerPanel.hpp"
#include "Panels/TestPanel.hpp"

struct GLFWwindow;

namespace QCreate
{
	class ImGuiLayer : public QMBT::Layer
	{
	  public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(const QMBT::TimeStep& timeStep) override;
		virtual void OnEvent(QMBT::Event& e) override;

		void BlockEvent(bool block) { m_BlockEvents = block; }

	  private:
		void SetDarkThemeColors();

		void BeginFrame(); // Sets up a new frame
		void EndFrame();   // Finished the frame and calls draw

		void SetUpDockspace();

	  private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

		TestPanel m_TestPanel;
		ProfilerPanel m_ProfilerPanel;
	};

} // namespace QCreate