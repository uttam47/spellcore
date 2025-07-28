#include <OpenGL/OpenGLObjectFactory.h>
#include <OpenGL/OpenGLRenderingContext.h>
#include <OpenGL/OpenGLSCShaderCompiler.h>
#include <OpenGL/OpenGLShaderProgram.h>
#include <OpenGL/OpenGLGpuBuffer.h>
#include <OpenGL/OpenGLVertexArray.h>


namespace AnalyticalApproach::Spellcore
{
    IRenderingContext *OpenGLObjectFactory::CreateRenderingContext()
    {
        return new OpenGLRenderingContext(); 
    }

    GPUBuffer *OpenGLObjectFactory::CreateVertexBuffer()
    {
        return new OpenGLGpuBuffer(); 
    }

    GeometryBuffer *OpenGLObjectFactory::CreateVertexArray()
    {
        return new OpenGLVertexArray(); 
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
}