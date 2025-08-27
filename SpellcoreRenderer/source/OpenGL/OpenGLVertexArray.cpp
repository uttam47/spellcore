#include <GL/glew.h>
#include <cassert>

#include "OpenGL/OpenGLVertexArray.h"
#include "core/GPUBufferLayout.h"

namespace AnalyticalApproach::Spellcore
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        }

        assert(false && "Unknown ShaderDataType!");
        return 0;
    }

    uint32_t OpenGLGeometryBuffer::GetId()
    {
        return _meshId;
    }

    uint64_t OpenGLGeometryBuffer::GetElementCount()
    {
        //TODO: Fetch from vertex buffer and return. 
        return 0; 
    }

    OpenGLGeometryBuffer::OpenGLGeometryBuffer()
    {
        glGenVertexArrays(1, &_meshId);
        glBindVertexArray(_meshId);
    }

    OpenGLGeometryBuffer::~OpenGLGeometryBuffer()
    {
        glDeleteVertexArrays(1, &_meshId);

        for (auto& vertexBuffer : _vertexBuffer)
        {
            if (vertexBuffer != nullptr)
            {
                delete vertexBuffer; 
                vertexBuffer = nullptr; 
            }
        }
        _vertexBuffer.clear(); 
    }

    void OpenGLGeometryBuffer::Bind() const
    {
        glBindVertexArray(_meshId);
    }

    void OpenGLGeometryBuffer::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLGeometryBuffer::AddAttributeBuffer(GPUBuffer *vertexBuffer)
    {
        assert(vertexBuffer && "GPUBuffer must not be null.");
        assert(vertexBuffer->GetLayout().GetElements().size() && "GPUBuffer has no layout!");

        Bind();
        vertexBuffer->Bind();

        const auto &layout = vertexBuffer->GetLayout();
        uint32_t index = 0;

        for (const auto &element : layout.GetElements())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void *>(static_cast<uintptr_t>(element.offset)));
            ++index;
        }

        _vertexBuffer.push_back(vertexBuffer);
    }
}
