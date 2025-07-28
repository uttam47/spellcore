#pragma once
#include <RHI/IRenderingContext.h>
#include <RHI/GPUBuffer.h>
#include <RHI/GeometryBuffer.h>
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
        virtual GPUBuffer *CreateVertexBuffer() = 0;
        virtual GeometryBuffer *CreateVertexArray() = 0; 
        virtual ITexture *CreateTexture() = 0;
        virtual ISCShaderCompiler *CreateSCShaderCompiler() = 0; 
        virtual IShaderProgram *CreateShaderProgram() = 0; 
    };
}