#include <stdarg.h>

#include <imgui/imgui.h>

#include <Qombat/Core.hpp>

#include "Mouse.hpp"

namespace QCreate
{
	namespace ImGuiHelper
	{
		inline void DrawHoverableRect(const ImVec2& a, const ImVec2& b, const ImColor& color, const char* tooltip, ...)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(a, b, color);

			if (ImGuiHelper::IsRectHovered(a, b))
			{
				va_list args;
				va_start(args, tooltip);
				ImGui::SetTooltipV(tooltip, args);
				va_end(args);
			}
		}


	}

} // namespace QCreate