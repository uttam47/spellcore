#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "ShaderDataType.h"

namespace AnalyticalApproach::Spellcore
{
    struct BufferElement
    {
        bool normalized;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset;
        std::string name;

        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string &name, bool normalized = false) : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0)
        {
        }

        uint32_t GetComponentCount() const { return GetShaderDataTypeComponentCount(type); }
    };

    class BufferLayout
    {
    private:
        std::vector<BufferElement> _elements;
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
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement> &elements) : _elements(elements)
        {
            CalculateOffsetAndStride();
        }

        BufferLayout(const std::vector<BufferElement> &elements)
            : _elements(elements)
        {
            CalculateOffsetAndStride();
        }

        const std::vector<BufferElement> &GetElements() const
        {
            return _elements;
        }

        uint32_t GetStride() const
        {
            return _stride;
        }
    };

}