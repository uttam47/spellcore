#pragma once

#include <RHI/IRendererBackendFactory.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLObjectFactory : public IRendererBackendFactory
    {
    public:
        ~OpenGLObjectFactory() = default;
        IRenderingContext *CreateRenderingContext() override;
        GPUBuffer *CreateVertexBuffer() override;
        GeometryBuffer *CreateVertexArray() override; 
        ITexture *CreateTexture() override;
        ISCShaderCompiler *CreateSCShaderCompiler() override;
        IShaderProgram *CreateShaderProgram() override;
    };
}