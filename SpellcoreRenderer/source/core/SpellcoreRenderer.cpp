#include <Logger.h>
#include <core/SpellcoreRenderer.h>
#include <core/RenderPipeline.h>
#include <core/SpellcoreShader.h>
#include <core/RenderingBackend.h>
#include <filesystem>

namespace AnalyticalApproach::Spellcore
{

    IRenderingContext* SpellcoreRenderer::s_RenderingContext = nullptr;
    RenderPipeline* SpellcoreRenderer::s_RenderPipeline = nullptr; 

    bool SpellcoreRenderer::Initialize(const RenderingSurfaceCreateInfo &surfaceInfo)
    {
        RenderingBackend::Initialize(GraphicsApi::OpenGL); 
        
        s_RenderingContext = RenderingBackend::Get()->CreateRenderingContext(); 

        if (!s_RenderingContext->Initialize(surfaceInfo))
        {
            LOG_ERROR("Failed to initialize rendering context!");
            delete s_RenderingContext;
            s_RenderingContext = nullptr;
            return false;
        }

        s_RenderPipeline = new RenderPipeline(); 
     
        return true;
    }

    void SpellcoreRenderer::Shutdown()
    {
        if (s_RenderingContext)
        {
            s_RenderingContext->Shutdown();
            delete s_RenderingContext;
            s_RenderingContext = nullptr;
        }
    }

    //TODO: Add a ShaderManager class to exclusively manage Shader Object's life span. 
    //Consider the below as well. 

    /*
    Need a ShaderEventChannel to manage their creation and loading.
    Introduce SpellcoreShaderRegistery to achive the above.
    To build a bridge between RenderingPipeline and SpellcoreShaderRegistery.
    */

    SpellcoreShader* SpellcoreRenderer::LoadShader(std::string shaderPath)
    {
        return new SpellcoreShader(shaderPath);
    }

    void SpellcoreRenderer::UseShader(SpellcoreShader* scShader)
    {
        s_RenderPipeline->LoadSCShader(scShader); 
    }

    void SpellcoreRenderer::BeginFrame()
    {
        if (s_RenderingContext)
            s_RenderingContext->BeginFrame();
    }

    void SpellcoreRenderer::EndFrame()
    {
        if (s_RenderingContext)
            s_RenderingContext->EndFrame();
    }
}