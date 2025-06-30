#pragma once
#include "glm/glm.hpp"
namespace AnalyticalApproach::WindowSystem
{

    struct WindowHandle
    {
        void *nativeHandle = nullptr; // Can point to HWND, X11 Window, etc.
        GLFWwindow *glfwWindow = nullptr; 

        WindowHandle() = default;
        explicit WindowHandle(void *nHandle, GLFWwindow* glfWindow)
            : nativeHandle(nHandle), glfwWindow(glfWindow){}

        bool IsValid() const { return nativeHandle != nullptr; }
    };

    class IWindowSystem
    {
    public:
        virtual ~IWindowSystem() = default;
        virtual WindowHandle CreateAppWindow(int width, int height, const char *title) = 0;
        virtual void DestroyAppWindow(WindowHandle window) = 0;
        virtual void PollEvents() = 0;
        virtual void *GetNativeHandle(WindowHandle window) = 0;
        virtual void RefreshFrame(WindowHandle window) = 0; 
        virtual glm::ivec2 GetWindowSize() = 0; 
    };

}