#include "Application.hpp"

#include "Log.hpp"
#include "Events/ApplicationEvent.hpp"

namespace QMBT
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        LOG_CORE_INFO("Started Run Loop");
        WindowResizeEvent e(1280, 720);
        LOG_CORE_INFO(e);
        while (true)
        {
        }
    }
}