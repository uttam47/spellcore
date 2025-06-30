#include <OpenGL/OpenGLObjectFactory.h>
#include <OpenGL/OpenGLRenderingContext.h>
#include <OpenGL/OpenGLSCShaderCompiler.h>
#include <OpenGL/OpenGLShaderProgram.h>
#include <OpenGL/OpenGLVertexBuffer.h>
#include <OpenGL/OpenGLVertexArray.h>


namespace AnalyticalApproach::Spellcore
{
    IRenderingContext *OpenGLObjectFactory::CreateRenderingContext()
    {
        return new OpenGLRenderingContext(); 
    }

    VertexBuffer *OpenGLObjectFactory::CreateVertexBuffer()
    {
        return new OpenGLVertexBuffer(); 
    }

    VertexArray *OpenGLObjectFactory::CreateVertexArray()
    {
        return new OpenGLVertexArray(); 
    }

    IndexBuffer *OpenGLObjectFactory::CreateIndexBuffer()
    {
        return nullptr; 
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