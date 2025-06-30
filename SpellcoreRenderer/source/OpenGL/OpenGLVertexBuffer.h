#pragma once

#include "RHI/VertexBuffer.h"

namespace AnalyticalApproach::Spellcore
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        //Sending a float pointer means that we are
        OpenGLVertexBuffer() = default;
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetLayout(const BufferLayout& layout) override;
        virtual const BufferLayout& GetLayout() const override;
        virtual void SetBufferData(float* data, uint32_t size) override;

    private:
        uint32_t _rendererID = 0;
        BufferLayout _layout;
    };
}
