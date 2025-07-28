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
        virtual void AddVertexBuffer(GPUBuffer*) = 0;
        virtual uint32_t GetId() = 0; 
        virtual uint64_t GetElementCount() = 0; 
    };
}