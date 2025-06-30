#include "SandboxApp.h"
#include "core/RenderingSurfaceCreateInfo.h"

namespace AnalyticalApproach
{
    using namespace WindowSystem;
    using namespace Spellcast;
    using namespace EventSystem;
    using namespace Spellcore; 

    SandboxApp::SandboxApp()
    {
        _windowEventChannel = EventManager::GetInstance().GetEventChannel<WindowEventChannel>();
        
        _windowHandle = _windowSystem.CreateAppWindow(1280, 720, "Spellcore Engine Window");

        glm::ivec2 windowSize = _windowSystem.GetWindowSize(); 
        RenderingSurfaceCreateInfo renderingSurfaceInfo{

            _windowHandle.nativeHandle, 
            windowSize.x,
            windowSize.y,
            false            
        }; 
        
        SpellcoreRenderer::Initialize(renderingSurfaceInfo); 
        _windowEventChannel->on_window_closed.subscribe(&SandboxApp::CloseApp, this);
        
        if (!_windowHandle.IsValid())
        {
            LOG_ERROR("Failed to create window");
        }
    }
    
    SandboxApp::~SandboxApp()
    {
        _windowEventChannel->on_window_closed.unsubscribe(&SandboxApp::CloseApp, this);
        _windowSystem.DestroyAppWindow(_windowHandle);
    }
    
    int SandboxApp::Run()
    {
        LOG_INFO("Starting the SandboxApp"); 

        if (!_windowHandle.IsValid())
            return -1;
        
        while (!_closeApp)
        {
            _windowSystem.PollEvents();
            SpellcoreRenderer::BeginFrame(); 
            SpellcoreRenderer::EndFrame(); 
        }

        LOG_INFO("Finished running the sandbox app"); 

        return 0;
    }

    void SandboxApp::CloseApp()
    {
        _closeApp = true; 
    }
    
}