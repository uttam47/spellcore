#pragma once
#include <RHI/IndexBuffer.h>
#include <RHI/IRenderingContext.h>
#include <RHI/VertexBuffer.h>
#include <RHI/VertexArray.h>
#include <RHI/ITexture.h>
#include <RHI/IShaderProgram.h>
#include <RHI/ISCShaderCompiler.h>


namespace AnalyticalApproach::Spellcore
{
    class IRendererBackendFactory
    {
    public:
        virtual ~IRendererBackendFactory() = default;
        virtual IRenderingContext *CreateRenderingContext() = 0;
        virtual VertexBuffer *CreateVertexBuffer() = 0;
        virtual GeometryBuffer *CreateVertexArray() = 0; 
        virtual IndexBuffer *CreateIndexBuffer() = 0;
        virtual ITexture *CreateTexture() = 0;
        virtual ISCShaderCompiler *CreateSCShaderCompiler() = 0; 
        virtual IShaderProgram *CreateShaderProgram() = 0; 
    };
}