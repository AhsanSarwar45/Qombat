// #pragma once

// #include "Xenos/Core/Window.h"
// #include "Xenos/Renderer/GraphicsContext.h"
// #include <GLFW/glfw3.h>

// namespace Xenos
// {

//     class WindowsWindow : public Window
//     {
//     public:
//         WindowsWindow(const WindowProperties &properties);
//         virtual ~WindowsWindow();

//         //Update GLFW stuff, swap buffers, poll the input events
//         void OnUpdate() override;

//         uint32_t GetWidth() const override { return m_Data.Width; }
//         uint32_t GetHeight() const override { return m_Data.Height; }

//         // Window attributes
//         void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }
//         void SetVSync(bool enabled) override;
//         bool IsVSync() const override;

//         inline virtual void *GetNativeWindow() const { return m_Window; }

//     private:
//         virtual void Init(const WindowProperties &properties);
//         virtual void Shutdown();

//     private:
//         GLFWwindow *m_Window;
//         GraphicsContext *m_Context;

//         //Struct to pass windows data to GLFW as custom user data
//         struct WindowData
//         {
//             std::string Title;
//             unsigned int Width, Height;
//             bool VSync;

//             EventCallbackFn EventCallback;
//         };

//         WindowData m_Data;
//     };

// }
