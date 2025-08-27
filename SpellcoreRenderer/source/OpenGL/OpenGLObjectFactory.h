#pragma once

#include <RHI/IRendererBackendFactory.h>
#include <RHI/RendereQueue.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLObjectFactory : public IRendererBackendFactory
    {
    public:
        ~OpenGLObjectFactory() = default;
        IRenderingContext *CreateRenderingContext() override;
        GPUBuffer *CreateGPUBuffer() override;
        GeometryBuffer *CreateGeometryBuffer() override; 
        ITexture *CreateTexture() override;
        ISCShaderCompiler *CreateSCShaderCompiler() override;
        IShaderProgram *CreateShaderProgram() override;
        RenderQueue* CreateRenderQueue() override;
    };
}