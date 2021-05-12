#include "LinuxWindow.hpp"
#include <QMBTPCH.hpp>

#include <glad/glad.h>

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

#include "Rendering/OpenGL/OpenGLContext.hpp"

namespace QMBT
{

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	//Initialize Windows (Current implementation: GLFW)
	void LinuxWindow::Init(const WindowProperties& properties)
	{
		m_Data.Title = properties.Title;
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;

		//XS_CORE_INFO("Creating window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);

		//Initialize GLFW
		if (!s_GLFWInitialized)
		{

			QMBT_CORE_VERIFY(glfwInit(), "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
			LOG_CORE_INFO("Initialized GLFW");
		}

		m_Window = glfwCreateWindow(static_cast<int>(properties.Width), static_cast<int>(properties.Height), m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		//m_Context = new OpenGLContext(m_Window);
		//m_Context->Init();

		QMBT_CORE_VERIFY(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Could not initialize GLAD!");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{

				MouseButtonClickedEvent event(button);
				data.EventCallback(event);

				// Double Click Implementation
				static auto before = std::chrono::system_clock::now();
				auto now = std::chrono::system_clock::now();
				double diff_ms = std::chrono::duration<double, std::milli>(now - before).count();
				before = now;

				static double xPosOld = 0, yPosOld = 0;
				double xPosNew, yPosNew;
				glfwGetCursorPos(window, &xPosNew, &yPosNew);

				static int buttonClicked = button;

				if (diff_ms > 10 && diff_ms < 500 && buttonClicked == button)
				{
					if (xPosNew == xPosOld && yPosNew == yPosOld)
					{
						MouseDoubleClickedEvent event(button);
						data.EventCallback(event);
					}
				}
				xPosOld = xPosNew;
				yPosOld = yPosNew;

				buttonClicked = button;
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(keyCode);
			data.EventCallback(event);
		});
	}

	LinuxWindow::LinuxWindow(const WindowProperties& properties)
	{
		Init(properties);

		LOG_CORE_INFO("Initialized Window {0} ({1} x {2})", properties.Title, properties.Width, properties.Height);
	}

	LinuxWindow::~LinuxWindow()
	{
		Shutdown();
	}

	Scope<Window> Window::Create(const WindowProperties& properties)
	{
		return CreateScope<LinuxWindow>(properties);
	}

	void LinuxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void LinuxWindow::OnUpdate()
	{
		PROFILE_FUNCTION(ProfileCategory::Window);
		//Process all pending events
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		//m_Context->SwapBuffers();
	}

	void LinuxWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

} // namespace QMBT