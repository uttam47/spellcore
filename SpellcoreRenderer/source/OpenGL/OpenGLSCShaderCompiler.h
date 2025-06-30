#pragma once
#include <core/SpellcoreShader.h>
#include <RHI/ISCShaderCompiler.h>
#include <RHI/IShaderProgram.h>
#include <unordered_map>
#include <string>
#include <functional>

class SpellcoreShader;
class ISCShaderCompiler;
class ShaderPipeline;

namespace AnalyticalApproach::Spellcore
{
    class OpenGLSCShaderCompiler : public ISCShaderCompiler
    {
    private:
        using GeneratorFunc = std::function<std::string(const std::string&)>;

        std::string GetStageSource(SpellcoreShader* spellcoreShader, ShaderType stage) const override;

        static std::string GenerateVertexShader(const std::string& scShaderSource);
        static std::string GenerateFragmentShader(const std::string& scShaderSource);
        static std::string GenerateTessEvalShader(const std::string& scShaderSource);
        static std::string GenerateTessControlShader(const std::string& scShaderSource);
        static std::string GenerateComputeShader(const std::string& scShaderSource);
        static std::string GenerateGeometryShader(const std::string& scShaderSource);

        static const std::unordered_map<ShaderType, GeneratorFunc> _generatorMap;

    public:
        BufferLayout* ExtractBufferLayout(SpellcoreShader* spellcoreShader) override;
        IShaderProgram* CreateShaderProgram(SpellcoreShader* spellcoreShader) override;
    };
} 
