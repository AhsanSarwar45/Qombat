// #include "QMBTPCH.hpp"
// #include "OpenGLContext.hpp"
// #include <GLFW/glfw3.h>

// namespace QMBT
// {
//     OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
//         : m_WindowHandle(windowHandle)
//     {
//         XS_CORE_ASSERT(windowHandle, "Window Handle is null!");
//     }
//     void OpenGLContext::Init()
//     {
//         glfwMakeContextCurrent(m_WindowHandle);

//         //Initialize Glad
//         int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//         XS_CORE_ASSERT(status, "Could not initialize Glad!");
//     }
//     void OpenGLContext::SwapBuffers()
//     {
//         glfwSwapBuffers(m_WindowHandle);
//     }
// }