#include <OpenGL/OpenGLSCShaderCompiler.h>
#include <core/SpellcoreShader.h>
#include <RHI/ISCShaderCompiler.h>
#include <RHI/IShaderProgram.h>
#include <OpenGL/OpenGLShaderProgram.h>

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <Logger.h>

static std::string ExtractBlock(const std::string &src, const std::string &blockName)
{
        const std::string openTag = blockName + "_START";
        const std::string closeTag = blockName + "_END";

        size_t start = src.find(openTag);
        if (start == std::string::npos)
                return ""; // block not found

        start += openTag.length();

        size_t end = src.find(closeTag, start);
        if (end == std::string::npos)
                return "";

        return src.substr(start, end - start);
}

namespace AnalyticalApproach::Spellcore
{
        const std::unordered_map<ShaderType, OpenGLSCShaderCompiler::GeneratorFunc> OpenGLSCShaderCompiler::_generatorMap = {
            {ShaderType::Vertex, GenerateVertexShader},
            {ShaderType::Fragment, GenerateFragmentShader},
            {ShaderType::TessControl, GenerateTessControlShader},
            {ShaderType::TessEvaluation, GenerateTessEvalShader},
            {ShaderType::Geometry, GenerateGeometryShader},
            {ShaderType::Compute, GenerateComputeShader}};

        BufferLayout *OpenGLSCShaderCompiler::ExtractBufferLayout(SpellcoreShader *spellcoreShader)
        {
                return new BufferLayout();
        }

        IShaderProgram *OpenGLSCShaderCompiler::CreateShaderProgram(SpellcoreShader *spelcoreShader)
        {
                OpenGLShaderProgram *shaderProgram = new OpenGLShaderProgram();

                for (ShaderType shaderType : {
                         ShaderType::Vertex,
                         ShaderType::Fragment,
                         ShaderType::Compute,
                         ShaderType::TessControl,
                         ShaderType::TessEvaluation,
                         ShaderType::Geometry})
                {
                        shaderProgram->AddStageSource(shaderType, GetStageSource(spelcoreShader, shaderType));
                }

                shaderProgram->Compile();
                return shaderProgram;
        }

        std::string OpenGLSCShaderCompiler::GetStageSource(SpellcoreShader *spellcoreShader, ShaderType stage) const
        {
                const auto &it = _generatorMap.find(stage);
                if (it != _generatorMap.end())
                {
                        return it->second(spellcoreShader->GetSource());
                }
        }

        std::string OpenGLSCShaderCompiler::GenerateVertexShader(const std::string &src)
        {
                return ExtractBlock(src, "VERTEX_PROCESSING_BLOCK");
        }

        std::string OpenGLSCShaderCompiler::GenerateFragmentShader(const std::string &src)
        {
                return ExtractBlock(src, "FRAGMENT_PROCESSING_BLOCK");
        }

        std::string OpenGLSCShaderCompiler::GenerateGeometryShader(const std::string &src)
        {
                return ExtractBlock(src, "GEOMETRY_PROCESSING_BLOCK");
        }

        std::string OpenGLSCShaderCompiler::GenerateTessControlShader(const std::string &src)
        {
                return ExtractBlock(src, "TESS_CONTROL_PROCESSING_BLOCK");
        }

        std::string OpenGLSCShaderCompiler::GenerateTessEvalShader(const std::string &src)
        {
                return ExtractBlock(src, "TESS_EVAL_PROCESSING_BLOCK");
        }

        std::string OpenGLSCShaderCompiler::GenerateComputeShader(const std::string &src)
        {
                return ExtractBlock(src, "COMPUTE_PROCESSING_BLOCK");
        }

}