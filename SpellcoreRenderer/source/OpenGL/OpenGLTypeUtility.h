#pragma once

#include <GL/glew.h>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/components/SCRenderTarget.h>
#include <assert.h>

namespace AnalyticalApproach::Spellcore
{
    static inline GLenum ToGLIndexType(SCDataType t)
    {
        switch (t)
        {
        case SCDataType::UShort:
            return GL_UNSIGNED_SHORT;
        case SCDataType::UInt:
            return GL_UNSIGNED_INT;
        default:
            return 0;
        }

        assert(false && "Unsupported Index Type!");
        return 0;
    }

    static inline GLenum ToGLPrimitiveType(SCPrimitive primitive)
    {
        switch (primitive)
        {
        case SCPrimitive::TRIANGLES:
            return GL_TRIANGLES;
        case SCPrimitive::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case SCPrimitive::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case SCPrimitive::LINES:
            return GL_LINES;
        case SCPrimitive::LINE_STRIP:
            return GL_LINE_STRIP;
        case SCPrimitive::LINE_LOOP:
            return GL_LINE_LOOP;
        case SCPrimitive::POINTS:
            return GL_POINTS;
        case SCPrimitive::PATCHES:
            return GL_PATCHES;
        }

        assert(false && "Unknown  Primitive Type!");
        return 0;
    }

    static inline GLenum ShaderDataTypeToOpenGLBaseType(SCDataType type)
    {
        switch (type)
        {
        case SCDataType::Float:
            return GL_FLOAT;
        case SCDataType::Float2:
            return GL_FLOAT;
        case SCDataType::Float3:
            return GL_FLOAT;
        case SCDataType::Float4:
            return GL_FLOAT;
        case SCDataType::Mat3:
            return GL_FLOAT;
        case SCDataType::Mat4:
            return GL_FLOAT;
        case SCDataType::Int:
            return GL_INT;
        case SCDataType::Int2:
            return GL_INT;
        case SCDataType::Int3:
            return GL_INT;
        case SCDataType::Int4:
            return GL_INT;
        case SCDataType::Bool:
            return GL_BOOL;
        }

        assert(false && "Unknown ShaderDataType!");
        return 0;
    }

    static inline GLenum GetShaderType(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::TessControl:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TessEvaluation:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderType::Compute:
            return GL_COMPUTE_SHADER; 
        }
        assert(false && "Unknown Shader Type: Defaulting to GL_VERTEX_SHADER!");
        return GL_VERTEX_SHADER;
    }

    static inline int ToGLSampleCount(SCRTSampleCount sc)
    {
        switch (sc)
        {
        case SCRTSampleCount::x1:  return 1;
        case SCRTSampleCount::x2:  return 2;
        case SCRTSampleCount::x4:  return 4;
        case SCRTSampleCount::x8:  return 8;
        case SCRTSampleCount::x16: return 16;
        }

        assert(false && "Unsupported Sample Count: Defaulting to 1!");
        return 1; 
    }

    static inline GLenum ToGLInternalFormat(SCRTFormat fmt)
    {

        switch (fmt)
        {
        case SCRTFormat::R8_UNorm:     return GL_R8;
        case SCRTFormat::RG8_UNorm:    return GL_RG8;
        case SCRTFormat::RGB8_UNorm:   return GL_RGB8;
        case SCRTFormat::RGBA8_UNorm:  return GL_RGBA8;

        case SCRTFormat::RGB8_sRGB:    return GL_SRGB8;
        case SCRTFormat::RGBA8_sRGB:   return GL_SRGB8_ALPHA8;

        case SCRTFormat::R16_UNorm:    return GL_R16;
        case SCRTFormat::RG16_UNorm:   return GL_RG16;
        case SCRTFormat::RGB16_UNorm:  return GL_RGB16;
        case SCRTFormat::RGBA16_UNorm: return GL_RGBA16;

        case SCRTFormat::R16F:         return GL_R16F;
        case SCRTFormat::RG16F:        return GL_RG16F;
        case SCRTFormat::RGB16F:       return GL_RGB16F;
        case SCRTFormat::RGBA16F:      return GL_RGBA16F;

        case SCRTFormat::R32F:         return GL_R32F;
        case SCRTFormat::RG32F:        return GL_RG32F;
        case SCRTFormat::RGB32F:       return GL_RGB32F;
        case SCRTFormat::RGBA32F:      return GL_RGBA32F;

        case SCRTFormat::D16:          return GL_DEPTH_COMPONENT16;
        case SCRTFormat::D24S8:        return GL_DEPTH24_STENCIL8;
        case SCRTFormat::D32F:         return GL_DEPTH_COMPONENT32F;

        case SCRTFormat::Unknown:
        default:
            return 0;
        }
        assert(false && "Unsupported Render Format: Defaulting to GL_RGBA8!");

        return GL_RGBA8;
    }

    static inline bool IsDepthStencilFormat(SCRTFormat fmt)
    {
        return fmt == SCRTFormat::D24S8;
    }

    static inline uint32_t ToGLAttachmentPoint(const SCAttachmentDesc& desc)
    {
        switch (desc.attachmentType)
        {
        case SCRTAttachmentType::Color:
            return GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(desc.attachmentIndex);

        case SCRTAttachmentType::Depth:
            // If format contains stencil, use combined attachment
            return IsDepthStencilFormat(desc.image.format) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

        case SCRTAttachmentType::Stencil:
            // If format contains depth too, prefer combined attachment
            return IsDepthStencilFormat(desc.image.format) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_STENCIL_ATTACHMENT;

        default:
            break;
        }

        // Should never happen
        assert(false && "Unknown SCRTAttachmentType");
        return GL_COLOR_ATTACHMENT0;
    }

    static inline int ToGLFilter(SCTextureFilters f)
    {
        switch (f)
        {
        case SCTextureFilters::Nearest:               return GL_NEAREST;
        case SCTextureFilters::Linear:                return GL_LINEAR;
        case SCTextureFilters::NearestMipmapNearest:  return GL_NEAREST_MIPMAP_NEAREST;
        case SCTextureFilters::LinearMipmapNearest:   return GL_LINEAR_MIPMAP_NEAREST;
        case SCTextureFilters::NearestMipmapLinear:   return GL_NEAREST_MIPMAP_LINEAR;
        case SCTextureFilters::LinearMipmapLinear:    return GL_LINEAR_MIPMAP_LINEAR;
        default:                                      return GL_NEAREST;
        }
    }

    static inline int ToGLWrap(SCTextureWrap w)
    {
        switch (w)
        {
        case SCTextureWrap::Repeat:         return GL_REPEAT;
        case SCTextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case SCTextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case SCTextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        default:                            return GL_CLAMP_TO_EDGE;
        }
    }
} // namespace AnalyticalApproach::Spellcore
