#pragma once

#include <vector>
#include <memory>
#include "RHI/GPUBuffer.h"
#include "RHI/GeometryBuffer.h"

#include <core/components/SpellcoreRenderDataTypes.h>

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
        void AddIndexBuffer(GPUBuffer* indexBuffer) override; 
        SCGeometryHandle GetId() override;
        
    private:
        SCGeometryHandle _geometryHandle;
        std::vector<GPUBuffer*> _vertexBuffer;
    };
}
