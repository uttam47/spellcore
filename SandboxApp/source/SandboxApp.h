#pragma once

#include "Events/UserInputChannel.h"
#include "Events/WindowEventChannel.h"
#include "WindowSystem/GLFW/GLFWWindowSystem.h"
#include "EventManager.h"
#include "Logger.h"
#include "core/SpellcoreRenderer.h"

namespace AnalyticalApproach
{
    class SandboxApp
    {
    public:
        SandboxApp();
        ~SandboxApp();

        void CloseApp(); 
        int Run();

    private:
        bool _closeApp = false; 
        WindowSystem::GLFWWindowSystem _windowSystem;
        WindowSystem::WindowHandle _windowHandle;
        WindowSystem::WindowEventChannel* _windowEventChannel = nullptr;
    };
}
