// #pragma once
// #include <algorithm>
// #include <vector>

// namespace QCreate
// {
// 	using PanelDrawFunction = void (*)();

// 	class PanelManager
// 	{
// 	  public:
// 		PanelManager();

// 		static inline void Register(PanelDrawFunction drawFunction) { m_DrawFunctions.push_back(drawFunction); }
// 		static inline void Unregister(PanelDrawFunction drawFunction)
// 		{
// 			m_DrawFunctions.erase(std::remove(m_DrawFunctions.begin(), m_DrawFunctions.end(), drawFunction), m_DrawFunctions.end());
// 		}

// 		static inline void DrawPanels()
// 		{
// 			for (auto drawFunction : m_DrawFunctions)
// 			{
// 				drawFunction();
// 			}
// 		}

// 	  private:
// 		static inline std::vector<PanelDrawFunction> m_DrawFunctions;
// 	};
// } // namespace QCreate