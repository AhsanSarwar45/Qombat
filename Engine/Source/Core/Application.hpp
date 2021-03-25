
namespace QMBT
{
    // Base class for all applications
    class Application
    {
    public:
        Application();
        virtual ~Application();

        // The main loop for the engine
        void Run();
    };

    /*
    To be defined in the client app.
    The client inherits from the Application class and implements the 
    CreateApplication function such that it returns a new instance of the 
    inherited client class
    */
    Application *CreateApplication();
}
