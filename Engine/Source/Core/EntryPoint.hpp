// Pragma once is used instead of header guards throught the codebase as
// they are less prone to bugs and is more convenient. The downside is that
// we cannot have multiple files with the same name across our project
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
    LOG_CORE_INFO("Initialized Core Logger");
    //QMBT::Logger::InitializeClientLogger();
}

int main(int argc, char *argv[])
{
    InitializeThirdParty();
    auto app = QMBT::CreateApplication();
    app->Run();
    delete app;
}