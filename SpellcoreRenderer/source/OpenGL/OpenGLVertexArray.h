#pragma once

#include <vector>
#include <memory>
#include "RHI/VertexBuffer.h"
#include "RHI/VertexArray.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLVertexArray: public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override; 
        void Unbind() const override;
        
        void AddVertexBuffer(VertexBuffer* vertexBuffer) override; 
        void SetIndexBuffer(IndexBuffer* indexBuffer) override;
        const IndexBuffer* GetIndexBuffer() const override;

    private:
        uint32_t m_RendererID;
        std::vector<VertexBuffer*> _vertexBuffer;
        IndexBuffer* _indexBuffer; 

    };
}
