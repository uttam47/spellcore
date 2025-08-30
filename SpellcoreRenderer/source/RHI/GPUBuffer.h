#pragma once

#include "core/GPUBufferLayout.h"
#include <memory>
#include "core/GPUBufferLayout.h"
#include <cstdint>
#include <vector>
#include <memory>

namespace AnalyticalApproach::Spellcore
{
    class GPUBuffer
    {
    public:
        virtual ~GPUBuffer() = default;

        virtual void Bind(uint32_t bindingPoint = 0) const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const GPUBufferLayout &layout) = 0;
        virtual const GPUBufferLayout &GetLayout() const = 0;

        virtual uint32_t GetBufferID() = 0; 

        // virtual uint32_t GetSize() const = 0;
        // virtual GPUBufferType GetBufferType() const = 0;
        // virtual GPUBufferUsageType GetUsageType() const = 0;

        template<typename T>
        void SetBufferData(const std::vector<T>& data, uint32_t offset = 0)
        {
            SetBufferDataInternal(static_cast<const void*>(data.data()), static_cast<uint32_t>(data.size() * sizeof(T)), offset);
        }

        template<typename T>
        void SetBufferData(const T* data, uint32_t count, uint32_t offset = 0)
        {
            SetBufferDataInternal(static_cast<const void*>(data), static_cast<uint32_t>(count * sizeof(T)), offset);
        }
    protected:
        virtual void SetBufferDataInternal(const void *data, uint32_t size, uint32_t offset = 0) = 0;
        virtual void SetAsVertexBuffer(const void *data, uint32_t size, uint32_t offset = 0) = 0; 
        virtual void SetAsUniformBuffer(const void *data, uint32_t size, uint32_t offset = 0) = 0; 
        virtual void SetAsShaderStorageBuffer(const void *data, uint32_t size, uint32_t offset = 0) = 0; 
    };
}
