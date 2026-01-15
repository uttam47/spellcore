#include <GL/glew.h>
#include <cassert>

#include <OpenGL/OpenGLGeometryBuffer.h>
#include <core/components/GPUBufferLayout.h>

namespace AnalyticalApproach::Spellcore
{
    static GLenum ShaderDataTypeToOpenGLBaseType(SCDataType type)
    {
        switch (type)
        {
        case SCDataType::Float:
            return GL_FLOAT;
        case SCDataType::Float2:
            return GL_FLOAT;
        case SCDataType::Float3:
            return GL_FLOAT;
        case SCDataType::Float4:
            return GL_FLOAT;
        case SCDataType::Mat3:
            return GL_FLOAT;
        case SCDataType::Mat4:
            return GL_FLOAT;
        case SCDataType::Int:
            return GL_INT;
        case SCDataType::Int2:
            return GL_INT;
        case SCDataType::Int3:
            return GL_INT;
        case SCDataType::Int4:
            return GL_INT;
        case SCDataType::Bool:
            return GL_BOOL;
        }

        assert(false && "Unknown ShaderDataType!");
        return 0;
    }

    SCGeometryHandle OpenGLGeometryBuffer::GetId()
    {
        return _geometryHandle;
    }

    OpenGLGeometryBuffer::OpenGLGeometryBuffer()
    {
        glGenVertexArrays(1, &_geometryHandle);
        glBindVertexArray(_geometryHandle);
    }

    OpenGLGeometryBuffer::~OpenGLGeometryBuffer()
    {

        glDeleteVertexArrays(1, &_geometryHandle);

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
        glBindVertexArray(_geometryHandle);
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

    void OpenGLGeometryBuffer::AddIndexBuffer(GPUBuffer* indexBuffer)
    {
        assert(indexBuffer && "GPUBuffer (index) must not be null.");
        
        const auto& layout = indexBuffer->GetLayout();
        assert(layout.gpuBufferSubType == GPUBufferSubType::INDEX_DATA && "AddIndexBuffer expects a buffer with GPUBufferSubType::INDEX_DATA");

        Bind();      
        indexBuffer->Bind();            
    }

}
