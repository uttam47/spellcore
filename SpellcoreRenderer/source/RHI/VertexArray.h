#pragma once
#include <memory>
#include <RHI/VertexBuffer.h>
#include <RHI/IndexBuffer.h>

namespace AnalyticalApproach::Spellcore
{
    class VertexArray
    {
        public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0; 
        virtual void AddVertexBuffer(VertexBuffer*) = 0;
        virtual void SetIndexBuffer(IndexBuffer*) = 0;
        virtual const IndexBuffer* GetIndexBuffer() const = 0;
    };
}