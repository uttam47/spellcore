#include <OpenGL/OpenGLObjectFactory.h>
#include <OpenGL/OpenGLRenderingContext.h>
#include <OpenGL/OpenGLSCShaderCompiler.h>
#include <OpenGL/OpenGLShaderProgram.h>
#include <OpenGL/OpenGLGpuBuffer.h>
#include <OpenGL/OpenGLGeometryBuffer.h>
#include <OpenGL/OpenGLRenderQueue.h>
#include <OpenGL/OpenGLRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
    IRenderingContext *OpenGLObjectFactory::CreateRenderingContext()
    {
        return new OpenGLRenderingContext(); 
    }

    GPUBuffer *OpenGLObjectFactory::CreateGPUBuffer()
    {
        return new OpenGLGpuBuffer(); 
    }

    GeometryBuffer *OpenGLObjectFactory::CreateGeometryBuffer()
    {
        return new OpenGLGeometryBuffer(); 
    }

    ISCShaderCompiler *OpenGLObjectFactory::CreateSCShaderCompiler()
    {
        return new OpenGLSCShaderCompiler();    
    }

    IShaderProgram *OpenGLObjectFactory::CreateShaderProgram()
    {
        return new OpenGLShaderProgram(); 
    }

    RenderQueue* OpenGLObjectFactory::CreateRenderQueue()
    {
        return new OpenGLRenderQueue(); 
    }

    IRenderTargetManager* OpenGLObjectFactory::CreateRenderTargetManager()
    {
        return new OpenGLRenderTargetManager(); 
    }
}