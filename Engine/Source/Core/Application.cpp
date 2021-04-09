#include "Application.hpp"

#include "Core/Logging/Logger.hpp"
#include "Events/ApplicationEvent.hpp"

namespace QMBT
{

	Application* Application::s_Instance = nullptr;
	Application::Application(const std::string& name)
		: m_Name(name), m_LayerStackAllocator(StackAllocator("LayerStack Allocator"))
	{
		LOG_CORE_INFO("Initialized Application {0}", m_Name);
		s_Instance = this;

		m_Window = Window::Create(WindowProperties(m_Name));
		QMBT_CORE_ASSERT(m_Window, "Window was not initialized properly!");
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		//XS_PROFILE_FUNCTION();
		LOG_CORE_INFO("Pushed Layer to Application LayerStack: {0}", layer->GetName());

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		//XS_PROFILE_FUNCTION();
		LOG_CORE_INFO("Pushed Overlay to Application LayerStack: {0}", layer->GetName());

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		//Create a new EventDispatcher with the received event
		EventDispatcher dispatcher(event);

		//If the received event is WindowCloseEvent, then OnWindowClose will be called
		//This is handled by the application only. Most other events are passed to the layers,
		//so they can do whatever they see fit
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			//Decrement the iterator and call its OnEvent
			(*--it)->OnEvent(event);

			//If event is handled by the current layer, do not propagate it further
			if (event.m_IsHandled)
				break;
		}
	}

		bool Application::OnWindowClose(const WindowCloseEvent& event)
	{
		LOG_CORE_INFO("Window Closed");
		m_Running = false;
		return true;
	}
} // namespace QMBT