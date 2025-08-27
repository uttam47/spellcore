#pragma once
#include <RHI/IRenderingContext.h>
#include <RHI/GPUBuffer.h>
#include <RHI/GeometryBuffer.h>
#include <RHI/ITexture.h>
#include <RHI/IShaderProgram.h>
#include <RHI/ISCShaderCompiler.h>
#include <RHI/RendereQueue.h>

namespace AnalyticalApproach::Spellcore
{
    class IRendererBackendFactory
    {
    public:
        virtual ~IRendererBackendFactory() = default;
        virtual IRenderingContext *CreateRenderingContext() = 0;
        virtual GPUBuffer *CreateGPUBuffer() = 0;
        virtual GeometryBuffer *CreateGeometryBuffer() = 0; 
        virtual ITexture *CreateTexture() = 0;
        virtual ISCShaderCompiler *CreateSCShaderCompiler() = 0; 
        virtual IShaderProgram *CreateShaderProgram() = 0; 
        virtual RenderQueue *CreateRenderQueue() = 0;
    };
}