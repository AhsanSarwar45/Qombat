#pragma once

#include "KeyCodes.hpp"
#include "MouseCodes.hpp"

namespace QMBT
{
	class Input
	{
	  public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonClicked(MouseCode button);
		static bool IsMouseButtonDoubleClicked(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
} // namespace QMBT