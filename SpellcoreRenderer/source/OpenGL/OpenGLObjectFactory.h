#pragma once

#include <RHI/IRendererBackendFactory.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLObjectFactory : public IRendererBackendFactory
    {
    public:
        ~OpenGLObjectFactory() = default;
        IRenderingContext *CreateRenderingContext() override;
        VertexBuffer *CreateVertexBuffer() override;
        VertexArray *CreateVertexArray() override; 
        IndexBuffer *CreateIndexBuffer() override;
        ITexture *CreateTexture() override;
        ISCShaderCompiler *CreateSCShaderCompiler() override;
        IShaderProgram *CreateShaderProgram() override;
    };
}