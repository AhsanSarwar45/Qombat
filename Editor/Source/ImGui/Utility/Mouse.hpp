#pragma once
#include <ImGui/imgui.h>

#include "Geometry.hpp"

namespace QCreate
{
	namespace ImGuiHelper
	{
		inline bool IsRectHovered(const ImVec2& a, const ImVec2& b)
		{
			ImGuiIO& io = ImGui::GetIO();
			return ImRectangleContainsPoint(a, b, io.MousePos);
		}
	} // namespace ImGuiHelper
} // namespace QCreate