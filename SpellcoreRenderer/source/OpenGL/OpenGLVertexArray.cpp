#include "OpenGL/OpenGLVertexArray.h"
#include "core/BufferLayout.h"
#include <GL/glew.h>
#include <cassert>

namespace AnalyticalApproach::Spellcore
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return GL_FLOAT;
        case ShaderDataType::Float2:  return GL_FLOAT;
        case ShaderDataType::Float3:  return GL_FLOAT;
        case ShaderDataType::Float4:  return GL_FLOAT;
        case ShaderDataType::Mat3:    return GL_FLOAT;
        case ShaderDataType::Mat4:    return GL_FLOAT;
        case ShaderDataType::Int:     return GL_INT;
        case ShaderDataType::Int2:    return GL_INT;
        case ShaderDataType::Int3:    return GL_INT;
        case ShaderDataType::Int4:    return GL_INT;
        case ShaderDataType::Bool:    return GL_BOOL;
        }

        assert(false && "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
    {
        assert(vertexBuffer && "VertexBuffer must not be null.");
        assert(vertexBuffer->GetLayout().GetElements().size() && "VertexBuffer has no layout!");

        Bind();
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        uint32_t index = 0;

        for (const auto& element : layout.GetElements())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(element.offset))
            );
            ++index;
        }

        _vertexBuffer.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
    {
        assert(indexBuffer && "IndexBuffer must not be null.");

        Bind();
        indexBuffer->Bind();

        _indexBuffer = indexBuffer;
    }

    const IndexBuffer* OpenGLVertexArray::GetIndexBuffer() const
    {
        return _indexBuffer;
    }
}
