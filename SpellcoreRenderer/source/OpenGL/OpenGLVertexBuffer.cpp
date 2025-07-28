#include "OpenGL/OpenGLVertexBuffer.h"
#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    void OpenGLVertexBuffer::SetBufferData(float* data, uint32_t size)
    {
        glGenBuffers(1, &_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &_rendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetLayout(const GPUBufferLayout& layout)
    {
        _layout = layout;
    }

    const GPUBufferLayout& OpenGLVertexBuffer::GetLayout() const
    {
        return _layout;
    }
}
