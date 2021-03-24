#pragma once

/* 
This functions is defined in the client. It will
return a new instance of a custom class inherited from Application.
*/
extern QMBT::Application *QMBT::CreateApplication();

int main(int argc, char *argv[])
{
    auto app = QMBT::CreateApplication();
    app->Run();
    delete app;
}