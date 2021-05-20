#include "TestPanel.hpp"

#include <ImGui/imgui.h>

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