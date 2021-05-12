#include "TestPanel.hpp"

#include <imgui/imgui.h>

#include <Qombat/Core.hpp>

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
		PROFILE_FUNCTION(QMBT::ProfileCategory::Editor);
		ImGui::Begin("Test");

		ImGui::End();
	}
} // namespace QCreate