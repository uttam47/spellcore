#pragma once
#include <core/GPUBufferLayout.h>
#include <core/SpellcoreShader.h>
#include <RHI/IShaderProgram.h>

namespace AnalyticalApproach::Spellcore
{

    class ISCShaderCompiler
    {
    private:
        virtual std::string GetStageSource(SpellcoreShader *spellcoreShader, ShaderType stage) const = 0;

    public:
        virtual ~ISCShaderCompiler() = default;
        virtual GPUBufferLayout *ExtractBufferLayout(SpellcoreShader *spellcoreShader) = 0;
        virtual IShaderProgram *CreateShaderProgram(SpellcoreShader *spellcoreShader) = 0;
    };

}