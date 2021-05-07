#include "ProfilerPanel.hpp"

#include <implot/implot.h>

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
		Instrumentor::Get().EndFrame();

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

				ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, bottomEdge + ImGuiStyleVar_FramePadding));

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("record", ImVec2(50, 20)))
		{
			if (Instrumentor::Get().IsRecording())
			{
				// LOG_CORE_INFO("Length: {0}", m_Data->size());
				// LOG_CORE_INFO("Name: {0}, Start: {1}, Elapsed: {2}, Thread: {3}",
				// 			  (*m_Data)[0]->Name, (*m_Data)[0]->Start.count(), (*m_Data)[0]->ElapsedTime.count(), (*m_Data)[0]->ThreadID);
				Instrumentor::Get().EndSession();
			}
			else
			{
				Instrumentor::Get().BeginSession();
				m_Frames = Instrumentor::Get().GetFrames();
			}
		}

		ImGui::Text("Is Recording: %d", Instrumentor::Get().IsRecording());

		//ImGui::BeginChild("scrolling", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);
		if (m_Frames)
		{

			int length = (*m_Frames).size();
			TimesArray* timesArray = Instrumentor::Get().GetTimes();

			float* otherTimes = &(*timesArray)[3][0];
			int otherLength = (*timesArray)[3].size();

			if (ImPlot::BeginPlot("My Plot"))
			{
				ImPlot::PlotLine("Time per frame", otherTimes, otherLength, 0.005);
				// 	ImGui::SetCursorPos(ImVec2((*m_Data)[i]->StartTime, yPos));
				// 	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
				// 	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
				// 	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
				// 	ImGui::Button((*m_Data)[i]->Name.c_str(), ImVec2((*m_Data)[i]->ElapsedTime, m_BarHeight));
				// 	// ImGui::Text("Name: %s, Start: %f, End: %f, Elapsed: %f, Thread: %d",
				// 	// 			(*m_Data)[i]->Name.c_str(),
				// 	// 			(*m_Data)[i]->StartTime,
				// 	// 			(*m_Data)[i]->EndTime,
				// 	// 			(*m_Data)[i]->ElapsedTime,`
				// 	// 			(*m_Data)[i]->ThreadID);
				// 	ImGui::PopStyleColor(3);
				// }
				ImPlot::EndPlot();
			}

			for (auto& frame : *m_Frames)
			{
				for (auto data : frame.Data)
				{
					ImGui::Text("Name: %s, Start: %f, End: %f, Elapsed: %f, Thread: %d",
								data->Name.c_str(),
								data->StartTime,
								data->EndTime,
								data->ElapsedTime,
								data->ThreadID);
				}
			}
		}
		//ImGui::EndChild();
		ImGui::End();

		Instrumentor::Get().BeginFrame();
	}

} // namespace QCreate