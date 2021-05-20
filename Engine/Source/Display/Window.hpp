#pragma once

#include "Core/Core.hpp"
#include "Core/Types/UniquePtr.hpp"
#include "Events/Events.hpp"

namespace QMBT
{
	using WindowDimension = uint16_t;

	struct WindowProperties
	{
		std::string Title;
		WindowDimension Width;
		WindowDimension Height;

		//Default settings for the window
		WindowProperties(
			const std::string& title = "Qombat",
			WindowDimension width = 1280,
			WindowDimension height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	// Implemented per platform
	class Window
	{
	  public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual WindowDimension GetWidth() const = 0;
		virtual WindowDimension GetHeight() const = 0;

		// For multi-screen setups
		virtual WindowDimension GetWidthVirtual() const = 0;
		virtual WindowDimension GetHeightVirtual() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual float GetTime() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static UniquePtr<Window> Create(const WindowProperties& props = WindowProperties());
	};

} // namespace QMBT
