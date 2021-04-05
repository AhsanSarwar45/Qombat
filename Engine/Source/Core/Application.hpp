#pragma once

#include "Core/Core.hpp"
#include "Display/Window.hpp"

#include "Core/LayerStack.hpp"
#include "Core/Memory/StackAllocator.hpp"
#include "Events/ApplicationEvent.hpp"

namespace QMBT
{
	// Base class for all applications
	class Application
	{
	  public:
		Application(const std::string& name = "Application");
		virtual ~Application();

		/**
		 * @brief The main loop for the engine.
		 * 
		 */
		void Run();

		/**
		 * @brief Event callback. Handles input, windows and application events.
		 * Passes all the events to all the layers in the LayerStack by calling their
		 * onEvent function.
		 * @param event The event to be handled
		 */
		void OnEvent(Event& event);

		/*
			Push an layer into the LayerStack.
		@remarks
			Pushed layer will be rendered above any previously pushed layers.
		@param layer: The layer to be pushed.
		*/
		void PushLayer(Layer* layer);

		/*
			Push an overlay into the LayerStack. 
		@remarks 
			Overlays are always rendered on top of the regular layers.
		@param layer: The layer to be pushed as an overlay.
		*/
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	  private:
		bool OnWindowClose(const WindowCloseEvent& event);

	  private:
		static Application* s_Instance;

		Scope<Window> m_Window;
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = true;

		std::string m_Name;
	};

	/*
To be defined in the client app.
The client inherits from the Application class and implements the
CreateApplication function such that it returns a new instance of the
inherited client class
*/
	Application* CreateApplication();
} // namespace QMBT
