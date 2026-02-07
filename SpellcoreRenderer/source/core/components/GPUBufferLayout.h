#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{

    struct GPUBufferElement
    {
        bool normalized;
        SCDataType type;
        uint32_t size;
        uint32_t offset;
        std::string name;

        GPUBufferElement() = default;
        GPUBufferElement(SCDataType type, const std::string &name, bool normalized = false) : 
            name(name), type(type), size(ShaderDataTypeSize(type)), offset(0)
        {}

        uint32_t GetComponentCount() const { return GetShaderDataTypeComponentCount(type); }
    };

    class GPUBufferLayout
    {
    private:
        std::vector<GPUBufferElement> _elements;
        uint32_t _stride;

        void CalculateOffsetAndStride()
        {
            uint32_t offset = 0;
            _stride = 0;
            for (auto &element : _elements)
            {
                element.offset = offset;
                offset += element.size;
                _stride += element.size;
            }
        }

    public:

        GPUBufferType gpuBufferType; 
        GPUBufferSubType gpuBufferSubType; 
        GPUBufferUsageType gpuBufferUsageType;  
        
        GPUBufferLayout() = default;

        GPUBufferLayout(const std::initializer_list<GPUBufferElement> &elements) : _elements(elements)
        {
            CalculateOffsetAndStride();
        }

        GPUBufferLayout(const std::vector<GPUBufferElement> &elements)
            : _elements(elements)
        {
            CalculateOffsetAndStride();
        }

        const std::vector<GPUBufferElement> &GetElements() const
        {
            return _elements;
        }

        uint32_t GetStride() const
        {
            return _stride;
        }
    };

}