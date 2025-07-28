#pragma once

#include <vector>
#include <memory>
#include "RHI/GPUBuffer.h"
#include "RHI/GeometryBuffer.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLVertexArray: public GeometryBuffer
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override; 
        void Unbind() const override;
        
        void AddVertexBuffer(GPUBuffer* vertexBuffer) override;         
        uint32_t GetId() override; 
        uint64_t GetElementCount() override; 

    private:
        uint32_t _meshId;
        std::vector<GPUBuffer*> _vertexBuffer;
    };
}
