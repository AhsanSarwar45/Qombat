#pragma once

#include <GLFW/glfw3.h>

#include "Core/Logging/Logger.hpp"
#include "Core/Types/UniquePtr.hpp"
#include "Display/Window.hpp"
#include "Rendering/GraphicsContext.hpp"

namespace QMBT
{

	class LinuxWindow : public Window
	{
	  public:
		explicit LinuxWindow(const WindowProperties& properties);
		virtual ~LinuxWindow();

		//Update GLFW stuff, swap buffers, poll the input events
		void OnUpdate() override;

		inline WindowDimension GetWidth() const override { return m_Data.Width; }
		inline WindowDimension GetHeight() const override { return m_Data.Height; }

		//For multi-screen setups
		inline WindowDimension GetWidthVirtual() const override { return m_Data.Width; }
		inline WindowDimension GetHeightVirtual() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return m_Data.VSync; }

		inline float GetTime() const override { return static_cast<float>(glfwGetTime()); }

		inline virtual void* GetNativeWindow() const { return m_Window; }

	  private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();

	  private:
		GLFWwindow* m_Window;
		UniquePtr<GraphicsContext> m_Context;

		//Struct to pass windows data to GLFW as custom user data
		struct WindowData
		{
			std::string Title;
			WindowDimension Width, Height;
			bool VSync;

			EventCallbackFunction EventCallback;
		};

		WindowData m_Data;
	};

} // namespace QMBT
