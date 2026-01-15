#pragma once

#include <cstdint>
#include <string>

namespace AnalyticalApproach::Spellcore
{
    using SCRenderTargetHandle = std::uint8_t;
    using SCRenderPassHandle = std::uint8_t;
    using SCMaterialHandle = std::uint32_t; 
    using SCTextureHandle = std::uint32_t; 
    using SCShaderHandle = std::uint16_t; 
    using SCImageHandle = std::uint32_t; 
    using SCGeometryHandle = std::uint32_t; 
    using SCSize = std::uint8_t; 

    enum class SCRenderTargetBindings
    {
        DEPTH, 
        COLOR_RGBA, 
        COLOR_RGB, 
        STENCIL 
    }; 

    enum class SCPrimitive : uint8_t
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN, // Optional
        LINES,
        LINE_STRIP,
        LINE_LOOP, // Optional
        POINTS,
        PATCHES
    };


    enum class SCDataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt,           // NEW (32-bit unsigned scalar)
        UShort,         // NEW (16-bit unsigned scalar)
        Mat3, Mat4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(SCDataType type)
    {
        switch (type)
        {
        case SCDataType::Float:   return 4;             // 4 bytes (32-bit float)
        case SCDataType::Float2:  return 4 * 2;         // 8 bytes  (2�float32)
        case SCDataType::Float3:  return 4 * 3;         // 12 bytes (3�float32)
        case SCDataType::Float4:  return 4 * 4;         // 16 bytes (4�float32)
        case SCDataType::Int:     return 4;             // 4 bytes (32-bit signed int)
        case SCDataType::Int2:    return 4 * 2;         // 8 bytes  (2�int32)
        case SCDataType::Int3:    return 4 * 3;         // 12 bytes (3�int32)
        case SCDataType::Int4:    return 4 * 4;         // 16 bytes (4�int32)
        case SCDataType::UInt:    return 4;             // 4 bytes (32-bit unsigned int)
        case SCDataType::UShort:  return 2;             // 2 bytes (16-bit unsigned short)
        case SCDataType::Mat3:    return 4 * 3 * 3;     // 36 bytes (3�3 float32; usually split into 3 vec3 attrs)
        case SCDataType::Mat4:    return 4 * 4 * 4;     // 64 bytes (4�4 float32; usually split into 4 vec4 attrs)
        case SCDataType::Bool:    return 1;             // 1 byte  (API-wise, vertex bools are typically bytes)
        default:                      return 0;
        }
    }

    static uint32_t GetShaderDataTypeComponentCount(SCDataType type)
    {
        switch (type)
        {
        case SCDataType::Float:  return 1;
        case SCDataType::Float2: return 2;
        case SCDataType::Float3: return 3;
        case SCDataType::Float4: return 4;
        case SCDataType::Int:    return 1;
        case SCDataType::Int2:   return 2;
        case SCDataType::Int3:   return 3;
        case SCDataType::Int4:   return 4;
        case SCDataType::Mat3:   return 3 * 3;
        case SCDataType::Mat4:   return 4 * 4;
        case SCDataType::Bool:   return 1;
        default: return 0;
        }
    }
}
