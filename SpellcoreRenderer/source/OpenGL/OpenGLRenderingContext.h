#pragma once

#include "RHI/IRenderingContext.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderingContext : public IRenderingContext
    {
    public:
        virtual bool Initialize(const RenderingSurfaceCreateInfo &surfaceInfo) override;
        virtual bool Shutdown() override;

        virtual bool BeginFrame() override;
        virtual bool EndFrame() override;

    private:
        HWND _windowHandle = nullptr;
        HDC _deviceContext = nullptr;
        HGLRC _glContext = nullptr;

        void PrintGLStatus(); 
    };

}