#include <Logger.h>
#include <core/SpellcoreRenderer.h>
#include <core/RenderPipeline.h>
#include <core/SpellcoreShader.h>
#include <core/RenderingBackend.h>

/*
    Need a ShaderEventChannel to manage their creation and loading. 
    Introduce SpellcoreShaderRegistery to achive the above.
    To build a bridge between RenderingPipeline and SpellcoreShaderRegistery. 
*/

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
        //Temporary: hardcoded here, need better shader managment. 

        //Figure out how to embed these default shaders in the app but still keeping it part of the SpellcoreShader. 
        SpellcoreShader* scSahder = new SpellcoreShader("C:/Users/uttam/OneDrive/Desktop/Development/Spellcore/SpellcoreRenderer/resources/DefaultShaders/BasicSpellcoreShader.scsh"); 
        s_RenderPipeline->LoadSCShader(scSahder); 


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