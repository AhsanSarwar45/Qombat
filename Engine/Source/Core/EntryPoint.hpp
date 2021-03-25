#pragma once

#include "Log.hpp"

/* 
This functions is defined in the client. It will
return a new instance of a custom class inherited from Application.
*/
extern QMBT::Application *QMBT::CreateApplication();

void InitializeThirdParty()
{
    // Initializing the Spdlog Logger
    QMBT::Logger::InitializeCoreLogger();
    QMBT_CORE_INFO("Initialized Core Logger");
    //QMBT::Logger::InitializeClientLogger();
}

int main(int argc, char *argv[])
{
    InitializeThirdParty();
    auto app = QMBT::CreateApplication();
    QMBT_CORE_INFO("Initialized Application");
    app->Run();
    delete app;
}