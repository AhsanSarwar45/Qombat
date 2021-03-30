#include "Application.hpp"

#include "Core/Log.hpp"
#include "Events/ApplicationEvent.hpp"

namespace QMBT
{

    Application::Application(const std::string &name)
        : m_Name(name)
    {
        m_Window = Window::Create(WindowProperties(name));
        m_Window->SetEventCallback(QMBT_BIND_EVENT_FUNCTION(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        QMBT_CORE_ASSERT(m_Window, "Window was not initialized!");
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(const Event &event)
    {
        LOG_CORE_TRACE(event);
    }
}