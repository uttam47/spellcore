#pragma once 

#include "core/GPUBufferLayout.h"
#include <memory>

namespace AnalyticalApproach::Spellcore
{
    
    class GPUBuffer
    {
        public: 

        virtual void Bind() const = 0; 
        virtual void Unbind() const = 0; 

        virtual void SetBufferData(float* data, uint32_t size) =0; 
        virtual void SetLayout(const GPUBufferLayout& layout) =0; 
        virtual const GPUBufferLayout& GetLayout() const =0; 

        virtual ~GPUBuffer() = default; 
    };
}