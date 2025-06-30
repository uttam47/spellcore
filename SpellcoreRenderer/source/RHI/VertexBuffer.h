#pragma once 

#include "core/BufferLayout.h"
#include <memory>

namespace AnalyticalApproach::Spellcore
{
    class VertexBuffer
    {
        public: 

        virtual void Bind() const = 0; 
        virtual void Unbind() const = 0; 

        virtual void SetBufferData(float* data, uint32_t size) =0; 
        virtual void SetLayout(const BufferLayout& layout) =0; 
        virtual const BufferLayout& GetLayout() const =0; 

        virtual ~VertexBuffer() = default; 
    };
}