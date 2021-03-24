#include "Application.hpp"
#include <iostream>

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
        while (true)
        {
            std::cout << "Running\n";
        }
    }
}