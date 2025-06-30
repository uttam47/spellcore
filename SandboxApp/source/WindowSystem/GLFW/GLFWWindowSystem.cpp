#include "GLFWWindowSystem.h"
#include <iostream>
#include <windows.h>

using namespace AnalyticalApproach::EventSystem; 

namespace AnalyticalApproach::WindowSystem
{
    // --- GLFW Callbacks ---


    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetUserInputChannel() && action == GLFW_PRESS)
        {
            system->GetUserInputChannel()->RaiseOnKeyPressed(key);
        }
    }

    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetUserInputChannel())
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (action == GLFW_PRESS)
            {
                system->GetUserInputChannel()->RaiseMouseButtonPressed(glm::vec2(xpos, ypos), button);
            }
        }
    }

    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetUserInputChannel())
        {
            // TODO: Maybe store it in the windows class, Maybe!
            static glm::vec2 lastPos(xpos, ypos);
            glm::vec2 currentPos(xpos, ypos);

            system->GetUserInputChannel()->RaiseMouseMoved(currentPos);
            system->GetUserInputChannel()->RaiseMouseDelta(currentPos - lastPos);

            lastPos = currentPos;
        }
    }

    static void WindowSizeCallback(GLFWwindow *window, int width, int height)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetWindowEventChannel())
        {
            system->GetWindowEventChannel()->RaiseOnWindowResized(glm::vec2(width, height));
        }
    }

    static void WindowPosCallback(GLFWwindow *window, int xpos, int ypos)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetWindowEventChannel())
        {
            system->GetWindowEventChannel()->RaiseWindowMoved(glm::vec2(xpos, ypos));
        }
    }

    static void WindowCloseCallback(GLFWwindow *window)
    {
        GLFWWindowSystem *system = static_cast<GLFWWindowSystem *>(glfwGetWindowUserPointer(window));
        if (system && system->GetWindowEventChannel())
        {
            system->GetWindowEventChannel()->RaiseWindowClosed();
        }
    }

    // --- End Callbacks ---

    GLFWWindowSystem::GLFWWindowSystem()
    {
        if (!glfwInit())
        {
            std::cerr << "[GLFWWindowSystem] Failed to initialize GLFW!\n";
            return; 
        }
        _windowEventChannel = EventManager::GetInstance().GetEventChannel<WindowEventChannel>(); 
        m_userInputChannel = EventManager::GetInstance().GetEventChannel<Spellcast::UserInputChannel>(); 
    }

    GLFWWindowSystem::~GLFWWindowSystem()
    {
        glfwTerminate();
    }

    WindowHandle GLFWWindowSystem::CreateAppWindow(int width, int height, const char *title)
    {
        _windowSize.x = width; 
        _windowSize.y = height; 
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
        GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window)
        {
            std::cerr << "[GLFWWindowSystem] Failed to create window!\n";
            return WindowHandle(nullptr, nullptr);
        }

        // --- Set the event callbacks ---
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);
        glfwSetWindowSizeCallback(window, WindowSizeCallback);
        glfwSetWindowPosCallback(window, WindowPosCallback);
        glfwSetWindowCloseCallback(window, WindowCloseCallback);

        HWND hwnd = glfwGetWin32Window(window); 
        glfwSetWindowUserPointer(window, this);

        return WindowHandle(hwnd,window);
    }

    void GLFWWindowSystem::DestroyAppWindow(WindowHandle window)
    {
        if (window.IsValid())
        {
            glfwDestroyWindow(static_cast<GLFWwindow *>(window.glfwWindow));
        }
    }

    void GLFWWindowSystem::PollEvents()
    {
        glfwPollEvents();
    }

    void *GLFWWindowSystem::GetNativeHandle(WindowHandle window)
    {
        if (!window.IsValid())
            return nullptr;

        return window.nativeHandle;
    }

    glm::ivec2 GLFWWindowSystem::GetWindowSize()
    {
        return _windowSize; 
    }

    void GLFWWindowSystem::RefreshFrame(WindowHandle window)
    {
        if (!window.IsValid())
            return;

        // // Clear color buffer
        // glClearColor(1, 1,1,1); 
        // glClear(GL_COLOR_BUFFER_BIT);

        // // Swap the window buffers
        // glfwSwapBuffers(static_cast<GLFWwindow *>(window.glfwWindow));
    }

}
