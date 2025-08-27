#include <OpenGL/OpenGLObjectFactory.h>
#include <OpenGL/OpenGLRenderingContext.h>
#include <OpenGL/OpenGLSCShaderCompiler.h>
#include <OpenGL/OpenGLShaderProgram.h>
#include <OpenGL/OpenGLGpuBuffer.h>
#include <OpenGL/OpenGLVertexArray.h>
#include <OpenGL/OpenGLRenderQueue.h>


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

    ITexture *OpenGLObjectFactory::CreateTexture()
    {
        return nullptr; 
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
}