#pragma once
#include <unordered_map>

#include <RHI/IShaderProgram.h>
#include <RHI/ISCShaderCompiler.h>
#include <core/SpellcoreShader.h>
#include <core/SpellcoreRenderingBackend.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreRenderPipeline
    {
    private:
        ISCShaderCompiler *_spellcoreShaderCompiler;
        std::unordered_map<SpellcoreShader *, IShaderProgram *> _shaderRegistery;

    public:
        SpellcoreRenderPipeline()
        {
            _spellcoreShaderCompiler = SpellcoreRenderingBackend::Get()->CreateSCShaderCompiler(); 
        }

        void Bind(SpellcoreShader *scShader);
        void Unbind(SpellcoreShader *scShader);

        void LoadSCShader(SpellcoreShader *scShader);
        void UnloadSCShader(SpellcoreShader *scShader);
    };
}