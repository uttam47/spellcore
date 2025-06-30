#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "WindowSystem/IWindowSystem.h"
#include "Events/UserInputChannel.h"
#include "Events/WindowEventChannel.h"
#include "EventManager.h"

namespace AnalyticalApproach::WindowSystem
{
    class GLFWWindowSystem : public IWindowSystem
    {
    private:
        Spellcast::UserInputChannel *m_userInputChannel = nullptr;
        WindowEventChannel *_windowEventChannel = nullptr;
        glm::ivec2 _windowSize; 

    public:
        GLFWWindowSystem();
        virtual ~GLFWWindowSystem();

        virtual WindowHandle CreateAppWindow(int width, int height, const char *title) override;
        virtual void DestroyAppWindow(WindowHandle window) override;
        virtual void PollEvents() override;
        virtual void *GetNativeHandle(WindowHandle window) override;
        virtual void RefreshFrame(WindowHandle window) override; 
        virtual glm::ivec2 GetWindowSize() override; 

        Spellcast::UserInputChannel *GetUserInputChannel()
        {
            return m_userInputChannel;
        }

        WindowEventChannel *GetWindowEventChannel()
        {
            return _windowEventChannel;
        }
    };
}
