#pragma once

#include <GL/glew.h>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <assert.h>

namespace AnalyticalApproach::Spellcore
{
    inline GLenum ToGLIndexType(SCDataType t)
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

    inline GLenum ToGLPrimitiveType(SCPrimitive primitive)
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

    inline GLenum ShaderDataTypeToOpenGLBaseType(SCDataType type)
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

    inline GLenum GetShaderType(ShaderType shaderType)
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
        assert(false && "Unknown Shader Type!");
        return 0;
    }
} // namespace AnalyticalApproach::Spellcore
