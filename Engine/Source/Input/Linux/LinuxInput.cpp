#include "QMBTPCH.hpp"

#include <GLFW/glfw3.h>

#include "../Input.hpp"
#include "Core/Application.hpp"

namespace QMBT
{

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonClicked(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}
	bool Input::IsMouseButtonDoubleClicked(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		static bool isDoubleClicked = false;

		if (glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS)
		{
			static auto before = std::chrono::system_clock::now();
			auto now = std::chrono::system_clock::now();
			double diff_ms = std::chrono::duration<double, std::milli>(now - before).count();
			before = now;

			static double xPosOld = 0, yPosOld = 0;
			double xPosNew, yPosNew;
			glfwGetCursorPos(window, &xPosNew, &yPosNew);

			static int buttonClicked = static_cast<int>(button);

			if (diff_ms > 10 && diff_ms < 500 && buttonClicked == static_cast<int>(button) && !isDoubleClicked)
			{
				if (xPosNew == xPosOld && yPosNew == yPosOld)
				{
					isDoubleClicked = true;
				}
			}
			xPosOld = xPosNew;
			yPosOld = yPosNew;
		}

		return isDoubleClicked;
	}
	glm::vec2 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return {(float)xPos, (float)yPos};
	}
	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

} // namespace QMBT
