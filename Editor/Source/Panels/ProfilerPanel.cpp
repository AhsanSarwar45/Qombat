#include "ProfilerPanel.hpp"

#include <implot/implot.h>
#include <implot/implot_internal.h>

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
		PROFILE_FUNCTION(ProfileCategory::Editor);
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

		if (Instrumentor::GetInstance().IsStopped())
		{
			if (ImGui::Button("Record", ImVec2(50, 20)))
			{
				Instrumentor::GetInstance().BeginSession();
				m_Frames = Instrumentor::GetInstance().GetFrames();
			}
		}
		else
		{
			if (ImGui::Button("Stop", ImVec2(50, 20)))
			{
				// LOG_CORE_INFO("Length: {0}", m_Data->size());
				// LOG_CORE_INFO("Name: {0}, Start: {1}, Elapsed: {2}, Thread: {3}",
				// 			  (*m_Data)[0]->Name, (*m_Data)[0]->Start.count(), (*m_Data)[0]->ElapsedTime.count(), (*m_Data)[0]->ThreadID);
				Instrumentor::GetInstance().EndSession();
				m_SelectedFrame = nullptr;
				m_Frames = nullptr;
				m_FrameMarkerPos = 0;
			}

			ImGui::SameLine();

			if (Instrumentor::GetInstance().IsPaused())
			{
				if (ImGui::Button("Resume", ImVec2(50, 20)))
				{
					Instrumentor::GetInstance().Resume();
				}
			}
			else
			{
				if (ImGui::Button("Pause", ImVec2(50, 20)))
				{
					Instrumentor::GetInstance().Pause();
				}
			}
		}

		//ImGui::BeginChild("scrolling", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);

		ImPlot::FitNextPlotAxes(false, true, false, false);
		ImPlot::SetNextPlotLimitsX(0, 240);
		if (ImPlot::BeginPlot("CPU time", "Frame No.", "Execution Time (us)"))
		{
			PROFILE_SCOPE("CPU Time", ProfileCategory::Editor);
			if (m_Frames)
			{

				// ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				// ImPlot::PlotShaded("Time per frame", otherTimes, otherLength,);
				// ImPlot::PopStyleVar();

				int length = (*m_Frames).size();
				TimesArray* timesArray = Instrumentor::GetInstance().GetCategoryTimes();
				double* totalTimes = Instrumentor::GetInstance().GetTotalTimes();

				Size numCategories = static_cast<Size>(ProfileCategory::Other) + 1;

				{
					PROFILE_SCOPE("Plotting", ProfileCategory::Editor);
					for (int i = 0; i < numCategories; i++)
					{

						double* times = &(*timesArray)[i][0];
						int length = (*timesArray)[i].size();

						PROFILE_SCOPE("ImPlot::PlotLine", ProfileCategory::Editor);
						ImPlot::PlotLine(Utility::EnumToString(Utility::IntegralToEnum<ProfileCategory>(i)).data(),
										 times, length, 1);
					}
				}

				ImPlot::PlotLine("Total",
								 totalTimes, length, 1);

				if (ImGui::IsMouseClicked(0) && ImPlot::IsPlotHovered())
				{
					m_FrameMarkerPos = std::round(ImPlot::GetPlotMousePos().x);
				}

				ImPlot::PlotVLines("##Marker", &m_FrameMarkerPos, 1);
			}

			ImPlot::EndPlot();
		}

		// Have to update m_SelectedFrame every frame instead of when marker is placed.
		// No idea why.
		if (m_Frames)
		{
			if (m_FrameMarkerPos >= 0 && m_FrameMarkerPos < (*m_Frames).size())
			{

				m_SelectedFrame = &((*m_Frames)[m_FrameMarkerPos]);
				m_SelectedFrameTime = Instrumentor::GetInstance().GetFrameTime(m_FrameMarkerPos);
			}
			else
			{
				m_SelectedFrame = nullptr;
				m_SelectedFrameTime = 0;
			}
		}

		if (m_SelectedFrame)
		{
			// Currently, the frame analyser works only for single-threaded applications
			//ImGui::BeginChild("Frame Analyser", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);

			// for (auto& row : m_FrameData)
			// {
			// 	Size length = row.size();
			// 	for (Size i = 0; i < length; i++)
			// 	{
			// 		ProfileData* data = row[i];
			// 		ImGui::SetCursorPosX((data->StartTime - row[0]->StartTime) * m_FrameAnalyserZoom);

			// 		ImGui::PushID(i);

			// 		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
			// 		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
			// 		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
			// 		ImGui::Button(data->Name.c_str(), ImVec2(data->ElapsedTime * m_FrameAnalyserZoom, m_BarHeight));

			// 		ImGui::PopStyleColor(3);

			// 		ImGui::PopID();
			// 		if (i < length - 1)
			// 		{
			// 			ImGui::SameLine();
			// 		}
			// 	}
			// }

			//ImGui::EndChild();

			double frameStartTime = m_SelectedFrame->Data.back()[0].StartTime;

			static ImPlotAxisFlags yAxisFlags = ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;

			ImPlot::FitNextPlotAxes(false, true, false, false);
			ImPlot::SetNextPlotLimitsY(-5, 0, ImGuiCond_Always);
			ImPlot::SetNextPlotLimitsX(0, m_SelectedFrame->Data[0].back().EndTime);
			if (ImPlot::BeginPlot("Frame Analyser", "Time since frame start (us)", (const char*)__null, ImVec2(-1, 150), 0, 0, yAxisFlags))
			{
				PROFILE_SCOPE("Frame Analyser", ProfileCategory::Editor);
				ImPlot::PushPlotClipRect();
				Size frameDataLength = m_SelectedFrame->Data.size();
				int index = 0;
				for (int r = frameDataLength - 1; r >= 0; r--)
				{
					auto& row = m_SelectedFrame->Data[r];

					Size length = row.size();
					for (Size i = 0; i < length; i++)
					{
						ProfileData& data = row[i];

						ImVec2 point1 = ImPlot::PlotToPixels(ImPlotPoint((data.StartTime - frameStartTime), -index));
						ImVec2 point2 = ImPlot::PlotToPixels(ImPlotPoint((data.EndTime - frameStartTime), -index - 1));
						ImPlot::GetPlotDrawList()->AddRectFilled(point1, point2, ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
						ImGui::PushClipRect(point1, point2, true);
						ImPlot::GetPlotDrawList()->AddText(point1, IM_COL32_WHITE, data.Name);
						ImGui::PopClipRect();
					}
					index++;
				}
				ImPlot::PopPlotClipRect();
				ImPlot::EndPlot();
			}

			ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit;
			if (ImGui::BeginTable("table", 6, tableFlags))
			{
				ImGui::TableSetupColumn("Function Name");
				ImGui::TableSetupColumn("Elapsed Time");
				ImGui::TableSetupColumn("Start Time");
				ImGui::TableSetupColumn("End Time");
				ImGui::TableSetupColumn("Thread");
				ImGui::TableSetupColumn("Group");

				ImGui::TableHeadersRow();

				for (auto& row : m_SelectedFrame->Data)
				{
					for (auto& data : row)

					{
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::Text(data.Name);

						ImGui::TableSetColumnIndex(1);
						ImGui::Text("%f", data.ElapsedTime);

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("%f", data.StartTime);

						ImGui::TableSetColumnIndex(3);
						ImGui::Text("%f", data.EndTime);

						ImGui::TableSetColumnIndex(4);
						ImGui::Text("%d", data.ThreadID);

						ImGui::TableSetColumnIndex(5);
						ImGui::Text("%s", Utility::EnumToString(data.Category).data());
					}
				}

				ImGui::EndTable();
			}
		}

		//ImGui::EndChild();
		ImGui::End();
	}

	void ProfilerPanel::OnEvent(Event& event)
	{
		PROFILE_FUNCTION(ProfileCategory::Editor);
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(ProfilerPanel::OnMouseScroll));
	}

	bool ProfilerPanel::OnMouseScroll(MouseScrolledEvent& event)
	{
		PROFILE_FUNCTION(ProfileCategory::Editor);
		m_FrameAnalyserZoom += event.GetYOffset() * 50;

		if (m_FrameAnalyserZoom < 1)
		{
			m_FrameAnalyserZoom = 1;
		}

		return false;
	}

	// void ProfilerPanel::SetFrameData()
	// {
	// 	m_FrameData.clear();
	// 	Size length = m_SelectedFrame->Data.size();
	// 	for (int i = 0; i < length; i++)
	// 	{
	// 		auto data = m_SelectedFrame->Data[i].get();

	// 		bool found = false;
	// 		Size rowIndex = 0;

	// 		while (!found)
	// 		{
	// 			if (rowIndex < m_FrameData.size())
	// 			{
	// 				auto& row = m_FrameData[rowIndex];
	// 				if (data->StartTime > row.back()->EndTime)
	// 				{
	// 					row.push_back(data);
	// 					found = true;
	// 				}
	// 			}
	// 			else
	// 			{
	// 				m_FrameData.push_back(std::vector<ProfileData*>());
	// 				m_FrameData.back().push_back(data);
	// 				found = true;
	// 			}

	// 			rowIndex++;
	// 		}
	// 	}
	// }

} // namespace QCreate