#pragma once

#include <vector>
#include <memory>
#include "RHI/VertexBuffer.h"
#include "RHI/VertexArray.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLVertexArray: public GeometryBuffer
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override; 
        void Unbind() const override;
        
        void AddVertexBuffer(VertexBuffer* vertexBuffer) override; 
        void SetIndexBuffer(IndexBuffer* indexBuffer) override;
        const IndexBuffer* GetIndexBuffer() const override;

        
        uint32_t GetId() override; 
        uint64_t GetElementCount() override; 

    private:
        uint32_t _meshId;
        std::vector<VertexBuffer*> _vertexBuffer;
        IndexBuffer* _indexBuffer; 

    };
}
