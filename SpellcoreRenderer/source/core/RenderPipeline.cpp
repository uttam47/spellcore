#include <core/RenderPipeline.h>

namespace AnalyticalApproach::Spellcore
{
    void RenderPipeline::Bind(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _shaderRegistery[scShader];
        shaderPipeline->Attach();
    }

    void RenderPipeline::SetRenderPass()
    {
    }

    void RenderPipeline::Unbind(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _shaderRegistery[scShader];
        shaderPipeline->Detach();
    }

    // This below two fucntion should be part of the SpellcoreShaderRegistery

    void RenderPipeline::LoadSCShader(SpellcoreShader *scShader)
    {
        IShaderProgram *shaderPipeline = _spellcoreShaderCompiler->CreateShaderProgram(scShader);
        GPUBufferLayout *bufferLayout = _spellcoreShaderCompiler->ExtractBufferLayout(scShader);
        scShader->SetBufferLayout(*bufferLayout);
        _shaderRegistery[scShader] = shaderPipeline;
        scShader->SetShaderHandle(shaderPipeline->GetShaderProgramHandle()); 
    }

    void RenderPipeline::UnloadSCShader(SpellcoreShader *scShader)
    {
    }
}