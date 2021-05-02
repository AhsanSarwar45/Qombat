#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace QCreate
{
	namespace ImGuiHelper
	{

		inline bool ImRectangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& point)
		{
			return ImTriangleContainsPoint(a, b, c, point) || ImTriangleContainsPoint(b, c, d, point);
		}
		inline bool ImRectangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& point)
		{
			return ImRectangleContainsPoint(a, ImVec2(b.x, a.y), ImVec2(a.x, b.y), b, point);
		}
	} // namespace ImGuiHelper
} // namespace QCreate