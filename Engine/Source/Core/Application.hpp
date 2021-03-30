#pragma once

#include "Core/Core.hpp"
#include "Display/Window.hpp"

namespace QMBT
{
    // Base class for all applications
    class Application
    {
    public:
        Application(const std::string &name = "Application");
        virtual ~Application();

        // The main loop for the engine
        void Run();

        void OnEvent(const Event &event);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
        std::string m_Name;
    };

    /*
    To be defined in the client app.
    The client inherits from the Application class and implements the 
    CreateApplication function such that it returns a new instance of the 
    inherited client class
    */
    Application *CreateApplication();
}
