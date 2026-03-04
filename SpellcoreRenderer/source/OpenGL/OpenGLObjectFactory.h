#pragma once

#include <RHI/IRendererBackendFactory.h>
#include <RHI/RendereQueue.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLObjectFactory : public IRendererBackendFactory
    {
    public:
        IRenderingContext *CreateRenderingContext() override;
        GPUBuffer *CreateGPUBuffer() override;
        GeometryBuffer *CreateGeometryBuffer() override; 
        ISCShaderCompiler *CreateSCShaderCompiler() override;
        IShaderProgram *CreateShaderProgram() override;
        RenderQueue* CreateRenderQueue() override;
        IRenderTarget* CreateRenderTarget() override; 
        //IRenderResourceManager* CreateRenderResourceManager() override;
    };
}