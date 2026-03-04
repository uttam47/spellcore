#include <Logger.h>
#include <filesystem>
#include <core/SpellcoreShader.h>
#include <core/SpellcoreRenderer.h>
#include <core/SpellcoreRenderingBackend.h>
#include <core/systems/SpellcoreRenderPipeline.h>
#include <core/systems/SCRendererResourceManager.h>

namespace AnalyticalApproach::Spellcore
{
    IRenderingContext* SpellcoreRenderer::s_RenderingContext = nullptr;
    SpellcoreRenderPipeline* SpellcoreRenderer::s_RenderPipeline = nullptr;
    RenderResourceRegistry* SpellcoreRenderer::s_RenderResourceRegistry = nullptr;
    SpellcoreRenderGraph* SpellcoreRenderer::s_SCRenderGraph = nullptr; 

    bool SpellcoreRenderer::Initialize(const RenderingSurfaceCreateInfo &surfaceInfo)
    {
        SpellcoreRenderingBackend::Initialize(GraphicsApi::OpenGL); 

        s_RenderResourceRegistry = RenderResourceRegistry::GetInstance(); 
        s_SCRenderGraph = new SpellcoreRenderGraph(); 
        
        s_RenderingContext = SpellcoreRenderingBackend::Get()->CreateRenderingContext();

        if (!s_RenderingContext->Initialize(surfaceInfo))
        {
            std::string erroMsg = "Failed to initialize rendering context!"; 
            LOG_ERROR(erroMsg);
            delete s_RenderingContext;
            s_RenderingContext = nullptr;
            throw std::runtime_error(erroMsg);
        }

        s_RenderPipeline = new SpellcoreRenderPipeline(); 
     
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
        {
            s_RenderingContext->BeginFrame();
        }

        {
            //Rebuild Frame graph and deduce frame resource dependency
        }
    }

    void SpellcoreRenderer::EndFrame()
    {
     
        if (s_RenderingContext)
        {
            s_RenderingContext->EndFrame();
        }
    }

    void SpellcoreRenderer::RenderFrame()
    {
        {
            //TODO: Instead of executing just one type of Render pass, execute them all as per their priority. 
            // Or give the option to do so individually, so that it has the flexibility to Render to any bound target. 
           // s_RenderQueue->Execute(RenderPassType::ForwardLighting);
        }
    }

    void SpellcoreRenderer::AddRenderPass(const std::string& renderPassName)
    {
        {
            //TODO: Instead of executing just one type of Render pass, execute them all as per their priority. 
            // Or give the option to do so individually, so that it has the flexibility to Render to any bound target. 
            //s_RenderQueue->Execute(RenderPassType::ForwardLighting);
        }
    }

    void SpellcoreRenderer::SubmitMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPassKey)
    {
        {
            //s_RenderQueue->Submit(); 
        }
    }

    void SpellcoreRenderer::RemoveMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPasskey)
    {
        {
           
        }
    }
}