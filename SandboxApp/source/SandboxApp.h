#pragma once

#include "Logger.h"
#include "Events/UserInputChannel.h"
#include "Events/WindowEventChannel.h"
#include "WindowSystem/GLFW/GLFWWindowSystem.h"
#include "EventManager.h"
#include "core/SpellcoreRenderer.h"
#include "Tests/IMGUITest/ImguiTest.h"
#include "Tests/ModelLoadingTest/ObjLoaderTest.h"
#include "Editor/Core/ResourceManager.h"

namespace AnalyticalApproach::Spellcore
{
    class SandboxApp
    {
    public:
        SandboxApp(std::vector<std::string> appParameters);
        ~SandboxApp();

        void CloseApp(); 
        int Run();


    private:
        bool _closeApp = false; 
        WindowSystem::GLFWWindowSystem _windowSystem;
        WindowSystem::WindowHandle _windowHandle;
        WindowSystem::WindowEventChannel* _windowEventChannel = nullptr;
        ResourceManager* _resourceManager; 

        //Test only
        ImguiTest _imguiTest;

    };
}
