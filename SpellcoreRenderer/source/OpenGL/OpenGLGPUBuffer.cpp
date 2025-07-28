#include "OpenGL/OpenGLGpuBuffer.h"
#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    void OpenGLGpuBuffer::SetBufferDataInternal(const void *data, uint32_t size, uint32_t offset)
    {
        // TODO: Decide what to do with the offset

        switch (_layout.gpuBufferType)
        {
        case GPUBufferType::VERTEX_BUFFER:
            SetAsVertexBuffer(data, size, offset);
            break;
        case GPUBufferType::UNIFORM_BUFFER:
            SetAsUniformBuffer(data, size, offset);
            break;
        case GPUBufferType::SHADER_STORAGE_BUFFER:
            SetAsShaderStorageBuffer(data, size, offset);
            break;
        default:
            break;
        }
    }

    void OpenGLGpuBuffer::SetAsVertexBuffer(const void *data, uint32_t size, uint32_t offset)
    {
        if (_bufferId == 0)
            glGenBuffers(1, &_bufferId);

        glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void OpenGLGpuBuffer::SetAsUniformBuffer(const void *data, uint32_t size, uint32_t offset)
    {
        if (_bufferId == 0)
            glGenBuffers(1, &_bufferId);

        glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW); // or STATIC depending on layout
    }

    void OpenGLGpuBuffer::SetAsShaderStorageBuffer(const void *data, uint32_t size, uint32_t offset)
    {
        if (_bufferId == 0)
            glGenBuffers(1, &_bufferId);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    OpenGLGpuBuffer::~OpenGLGpuBuffer()
    {
        glDeleteBuffers(1, &_bufferId);
    }

    void OpenGLGpuBuffer::Bind(uint32_t bindingPoint) const
    {
        switch (_layout.gpuBufferType)
        {
        case GPUBufferType::VERTEX_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
            break;
        case GPUBufferType::UNIFORM_BUFFER:
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _bufferId);
            break;
        case GPUBufferType::SHADER_STORAGE_BUFFER:
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, _bufferId);
            break;
        }
    }

    void OpenGLGpuBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLGpuBuffer::SetLayout(const GPUBufferLayout &layout)
    {
        _layout = layout;
    }

    const GPUBufferLayout &OpenGLGpuBuffer::GetLayout() const
    {
        return _layout;
    }
}
