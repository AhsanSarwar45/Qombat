#include "TestPanel.hpp"

#include <imgui/imgui.h>

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
		ImGui::Begin("Test");

		ImGui::End();
	}
} // namespace QCreate