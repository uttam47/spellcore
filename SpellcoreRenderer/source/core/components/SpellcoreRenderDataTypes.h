#pragma once

#include <cstdint>
#include <string>

namespace AnalyticalApproach::Spellcore
{
    using SCRenderTargetHandle = std::uint32_t;
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


    enum class SCDataType: uint8_t
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt,           // NEW (32-bit unsigned scalar)
        UShort,         // NEW (16-bit unsigned scalar)
        Mat3, Mat4,
        Bool
    };

    enum class SCImageUsage : uint32_t
    {
        ColorAttachment = 1 << 0,
        DepthStencil = 1 << 1,
        Sampled = 1 << 2,
        Storage = 1 << 3,
        CopySrc = 1 << 4,
        CopyDst = 1 << 5
    };

    enum class SCRTFormat : uint8_t
    {
        Unknown = 0,

        // 8-bit normalized color formats
        R8_UNorm,
        RG8_UNorm,
        RGB8_UNorm,
        RGBA8_UNorm,

        // 8-bit sRGB formats (shader-visible gamma correction)
        RGB8_sRGB,
        RGBA8_sRGB,

        // 16-bit normalized
        R16_UNorm,
        RG16_UNorm,
        RGB16_UNorm,
        RGBA16_UNorm,

        // 16-bit float (HDR / lighting)
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        // 32-bit float (G-buffer, compute, storage images)
        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        // Depth / stencil
        D16,
        D24S8,
        D32F,
    };

    enum class SCRTAttachmentType : uint8_t
    {
        Color = 0,
        Depth,
        Stencil
    };

    enum class SCRTSampleCount : uint8_t
    {
        x1,
        x2,
        x4,
        x8,
        x16
    };

    enum class SCImageType : uint8_t
    {
        Tex1D,
        Tex2D,
        Tex3D,
        Tex1DArray,
        Tex2DArray,
        TexCube,
        TexCubeArray
    };
    
    //TODO: Move these data description in their specific files. 

    enum class SCPixelFormat : uint8_t
    {
        Unknown = 0,

        // 8-bit normalized integer formats (most common for images)
        R8,
        RG8,
        RGB8,
        RGBA8,

        // 16-bit integer formats
        R16,
        RG16,
        RGB16,
        RGBA16,

        // 16-bit float formats (HDR pipelines)
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        // 32-bit float formats (rare, heavy)
        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        // Special / utility
        BGRA8,      // common on Windows image loaders
    };

    enum class SCTextureFilters : uint8_t
    {
        Nearest = 0,
        Linear,

        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class SCTextureWrap : uint8_t
    {
        Repeat = 0,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
    
    inline uint8_t ShaderDataTypeSize(SCDataType type)
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

    inline uint8_t GetShaderDataTypeComponentCount(SCDataType type)
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

    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        TessControl,
        TessEvaluation,
        Compute
    }; 


    enum class GPUBufferType
    {
        VERTEX_DATA_BUFFER = 0,
        UNIFORM_DATA_BUFFER,
        SHADER_STORAGE_DATA_BUFFER,
    };

    enum class GPUBufferSubType
    {
        VERTEX_DATA = 0,
        INDEX_DATA,
        NONE
    };

    enum class GPUBufferUsageType
    {
        STATIC = 0,
        STREAM,
        DYNAMIC,
        NONE
    };

    enum class SCMipLevels : uint8_t
    {
        Auto = 0,

        L1 = 1,
        L2 = 2,
        L3 = 3,
        L4 = 4,
        L5 = 5,
        L6 = 6,
        L7 = 7,
        L8 = 8,
        L9 = 9,
        L10 = 10,
        L11 = 11,
        L12 = 12,
        L13 = 13,
        L14 = 14,
        L15 = 15,
        L16 = 16, // enough for up to 65536 textures
    };

}
