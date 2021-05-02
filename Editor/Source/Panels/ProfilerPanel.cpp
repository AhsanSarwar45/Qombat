#include "ProfilerPanel.hpp"

#include <Qombat/Utility.hpp>

#include "ImGui/Utility/Colors.hpp"
#include "ImGui/Utility/Draw.hpp"
#include "ImGui/Utility/Geometry.hpp"

namespace QCreate
{
	ProfilerPanel::ProfilerPanel()
		: m_BarHeight(30), m_Colors(10)
	{
	}

	ProfilerPanel::~ProfilerPanel()
	{
	}

	void ProfilerPanel::Draw()
	{
		ImGui::Begin("Profiler");

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const AllocatorVector& allocators = MemoryManager::GetInstance().GetAllocators();
		const Size totalBudget = MemoryManager::GetInstance().GetApplicationMemoryBudget();
		const Size totalAllocated = MemoryManager::GetInstance().GetTotalAllocatedSize();

		ImGui::Text("Total Memory Allocation: ");
		ImGui::SameLine();
		ImGui::Text("%s/%s", QMBT::Utility::ToReadable(totalAllocated).c_str(), QMBT::Utility::ToReadable(totalBudget).c_str());

		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		float totalWidth = ImGui::GetContentRegionAvail().x;
		float bottomEdge = cursorPos.y + m_BarHeight;

		m_Colors.Reset();

		ImGuiHelper::DrawHoverableRect(cursorPos, ImVec2(cursorPos.x + totalWidth, bottomEdge),
									   m_Colors.GetRandomColor(), "Unallocated: %s",
									   QMBT::Utility::ToReadable(totalBudget - totalAllocated).c_str());

		ImVec2 startPos = cursorPos;
		for (auto allocator : allocators)
		{
			float width = (static_cast<float>(allocator->TotalSize) / static_cast<float>(totalBudget)) * totalWidth;

			ImGuiHelper::DrawHoverableRect(startPos, ImVec2(startPos.x + width, bottomEdge), m_Colors.GetRandomColor(),
										   "%s: %s", allocator->DebugName.c_str(),
										   QMBT::Utility::ToReadable(allocator->TotalSize).c_str());
			startPos.x += width;
		}

		ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, bottomEdge + ImGuiStyleVar_FramePadding));

		static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		for (auto allocator : allocators)
		{
			bool open = ImGui::TreeNodeEx(allocator->DebugName.c_str(), treeNodeFlags);
			if (open)
			{
				ImGui::Text("Memory Usage: ");
				ImGui::SameLine();
				ImGui::Text("%s/%s", QMBT::Utility::ToReadable(allocator->UsedSize).c_str(), QMBT::Utility::ToReadable(allocator->TotalSize).c_str());

				cursorPos = ImGui::GetCursorScreenPos();
				totalWidth = ImGui::GetContentRegionAvail().x;
				bottomEdge = cursorPos.y + m_BarHeight;

				ImGuiHelper::DrawHoverableRect(cursorPos, ImVec2(cursorPos.x + totalWidth, bottomEdge), m_Colors.GetRandomColor(),
											   "Available: %s", QMBT::Utility::ToReadable(allocator->TotalSize - allocator->UsedSize).c_str());

				float width = (static_cast<float>(allocator->UsedSize) / static_cast<float>(allocator->TotalSize)) * totalWidth;

				ImGuiHelper::DrawHoverableRect(cursorPos, ImVec2(cursorPos.x + width, bottomEdge), m_Colors.GetRandomColor(),
											   "Used: %s", QMBT::Utility::ToReadable(allocator->UsedSize).c_str());

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
} // namespace QCreate