#pragma once

#include "RHI/GPUBuffer.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLVertexBuffer : public GPUBuffer
    {
    public:
        //Sending a float pointer means that we are
        OpenGLVertexBuffer() = default;
        virtual ~OpenGLVertexBuffer();

        virtual void Bind(uint32_t bindingPoint = 0) const override;
        virtual void Unbind() const override;

        virtual void SetLayout(const GPUBufferLayout& layout) override;
        virtual const GPUBufferLayout& GetLayout() const override;

        virtual void SetBufferDataInternal(const void *data, uint32_t size, uint32_t offset = 0) override;
    private:
        uint32_t _rendererID = 0;
        GPUBufferLayout _layout;
    };
}
