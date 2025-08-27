#pragma once

#include <RHI/IRendererBackendFactory.h>

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
    };
}