#pragma once
#include <unordered_map>

#include <core/SpellcoreShader.h>
#include <RHI/IShaderProgram.h>
#include <RHI/ISCShaderCompiler.h>
#include <core/RenderingBackend.h>

namespace AnalyticalApproach::Spellcore
{
    class RenderPipeline
    {
    private:
        ISCShaderCompiler *_spellcoreShaderCompiler;
        std::unordered_map<SpellcoreShader *, IShaderProgram *> _shaderRegistery;

    public:
        RenderPipeline()
        {
            _spellcoreShaderCompiler = RenderingBackend::Get()->CreateSCShaderCompiler(); 
        }

        void Bind(SpellcoreShader *scShader);
        void SetRenderPass();
        void Unbind(SpellcoreShader *scShader);

        void LoadSCShader(SpellcoreShader *scShader);
        void UnloadSCShader(SpellcoreShader *scShader);
    };
}