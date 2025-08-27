#pragma once

#include <vector>
#include <memory>
#include "RHI/GPUBuffer.h"
#include "RHI/GeometryBuffer.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLGeometryBuffer: public GeometryBuffer
    {
    public:
        OpenGLGeometryBuffer();
        ~OpenGLGeometryBuffer();

        void Bind() const override; 
        void Unbind() const override;
        
        void AddAttributeBuffer(GPUBuffer* vertexBuffer) override;         
        uint32_t GetId() override; 
        uint64_t GetElementCount() override; 

    private:
        uint32_t _meshId;
        std::vector<GPUBuffer*> _vertexBuffer;
    };
}
