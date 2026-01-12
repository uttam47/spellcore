#include <core/RenderPipeline.h>

namespace AnalyticalApproach::Spellcore
{
    void SpellcoreRenderPipeline::Bind(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _shaderRegistery[scShader];
        shaderPipeline->Attach();
    }

    void SpellcoreRenderPipeline::SetRenderPass()
    {
    }

    void SpellcoreRenderPipeline::Unbind(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _shaderRegistery[scShader];
        shaderPipeline->Detach();
    }

    // This below two fucntion should be part of the SpellcoreShaderRegistery

    void SpellcoreRenderPipeline::LoadSCShader(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _spellcoreShaderCompiler->CreateShaderProgram(scShader);
        GPUBufferLayout *bufferLayout = _spellcoreShaderCompiler->ExtractBufferLayout(scShader);
        scShader->SetBufferLayout(*bufferLayout);
        _shaderRegistery[scShader] = shaderPipeline;
        scShader->SetShaderHandle(shaderPipeline->GetShaderProgramHandle()); 
    }

    void SpellcoreRenderPipeline::UnloadSCShader(SpellcoreShader *scShader)
    {
    }
}