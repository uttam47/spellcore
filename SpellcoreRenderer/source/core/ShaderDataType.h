#pragma once

#include <cstdint>
#include <string>

namespace AnalyticalApproach::Spellcore
{
    enum class ShaderDataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt,           // NEW (32-bit unsigned scalar)
        UShort,         // NEW (16-bit unsigned scalar)
        Mat3, Mat4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return 4;             // 4 bytes (32-bit float)
        case ShaderDataType::Float2:  return 4 * 2;         // 8 bytes  (2�float32)
        case ShaderDataType::Float3:  return 4 * 3;         // 12 bytes (3�float32)
        case ShaderDataType::Float4:  return 4 * 4;         // 16 bytes (4�float32)
        case ShaderDataType::Int:     return 4;             // 4 bytes (32-bit signed int)
        case ShaderDataType::Int2:    return 4 * 2;         // 8 bytes  (2�int32)
        case ShaderDataType::Int3:    return 4 * 3;         // 12 bytes (3�int32)
        case ShaderDataType::Int4:    return 4 * 4;         // 16 bytes (4�int32)
        case ShaderDataType::UInt:    return 4;             // 4 bytes (32-bit unsigned int)
        case ShaderDataType::UShort:  return 2;             // 2 bytes (16-bit unsigned short)
        case ShaderDataType::Mat3:    return 4 * 3 * 3;     // 36 bytes (3�3 float32; usually split into 3 vec3 attrs)
        case ShaderDataType::Mat4:    return 4 * 4 * 4;     // 64 bytes (4�4 float32; usually split into 4 vec4 attrs)
        case ShaderDataType::Bool:    return 1;             // 1 byte  (API-wise, vertex bools are typically bytes)
        default:                      return 0;
        }
    }

    static uint32_t GetShaderDataTypeComponentCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:  return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Int:    return 1;
        case ShaderDataType::Int2:   return 2;
        case ShaderDataType::Int3:   return 3;
        case ShaderDataType::Int4:   return 4;
        case ShaderDataType::Mat3:   return 3 * 3;
        case ShaderDataType::Mat4:   return 4 * 4;
        case ShaderDataType::Bool:   return 1;
        default: return 0;
        }
    }
}
