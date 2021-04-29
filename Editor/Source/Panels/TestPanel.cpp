#include "TestPanel.hpp"

#include <imgui/imgui.h>

#include <Core/Core.hpp>

namespace QCreate
{
	TestPanel::TestPanel()
	{
	}

	TestPanel::~TestPanel()
	{
	}

	void TestPanel::Draw()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Test");

		ImGui::End();
	}
} // namespace QCreate