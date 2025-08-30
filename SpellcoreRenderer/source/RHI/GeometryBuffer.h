#pragma once
#include <memory>
#include <RHI/GPUBuffer.h>

namespace AnalyticalApproach::Spellcore
{
    class GeometryBuffer
    {
        public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0; 
        virtual void AddAttributeBuffer(GPUBuffer*) = 0;
        virtual void AddIndexBuffer(GPUBuffer*) = 0; 
        virtual uint32_t GetId() = 0; 
    };
}