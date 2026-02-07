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
    RenderQueue* SpellcoreRenderer::s_RenderQueue = nullptr;
    RenderResourceRegistry* SpellcoreRenderer::s_RenderResourceRegistry = nullptr;

    bool SpellcoreRenderer::Initialize(const RenderingSurfaceCreateInfo &surfaceInfo)
    {
        SpellcoreRenderingBackend::Initialize(GraphicsApi::OpenGL); 

        s_RenderResourceRegistry = RenderResourceRegistry::GetInstance(); 
        
        s_RenderingContext = SpellcoreRenderingBackend::Get()->CreateRenderingContext();
        s_RenderQueue = SpellcoreRenderingBackend::Get()->CreateRenderQueue();

        if (!s_RenderingContext->Initialize(surfaceInfo))
        {
            LOG_ERROR("Failed to initialize rendering context!");
            delete s_RenderingContext;
            s_RenderingContext = nullptr;
            return false;
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
        {
            s_RenderingContext->BeginFrame();
        }

        if (s_RenderQueue)
        {
            s_RenderQueue->Clear(); 
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
        if (s_RenderQueue)
        {
            //TODO: Instead of executing just one type of Render pass, execute them all as per their priority. 
            // Or give the option to do so individually, so that it has the flexibility to Render to any bound target. 
           // s_RenderQueue->Execute(RenderPassType::ForwardLighting);
        }
    }

    void SpellcoreRenderer::AddRenderPass(const std::string& renderPassName)
    {

        if (s_RenderQueue)
        {
            //TODO: Instead of executing just one type of Render pass, execute them all as per their priority. 
            // Or give the option to do so individually, so that it has the flexibility to Render to any bound target. 
            //s_RenderQueue->Execute(RenderPassType::ForwardLighting);
        }
    }

    void SpellcoreRenderer::SubmitMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPassKey)
    {
        if (s_RenderQueue)
        {
            //s_RenderQueue->Submit(); 
        }
    }

    void SpellcoreRenderer::RemoveMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPasskey)
    {
        if (s_RenderQueue)
        {
           
        }
    }

    uint32_t SpellcoreRenderer::UploadGeometry(const SCGeometryData& scGeometryData)
    {
       return s_RenderResourceRegistry->CreateGeometryResource(scGeometryData);
    }

    bool SpellcoreRenderer::UpdateGeometry(const SCGeometryHandle& scGeoHandle, const SCGeometryData& scGeoemtryData)
    {
        return s_RenderResourceRegistry->UpdateGeometryResource(scGeoHandle, scGeoemtryData);
    }

    void SpellcoreRenderer::ReleaseGeometry(SCGeometryHandle& scGeoHandle)
    {
        s_RenderResourceRegistry->DestroyGeometryResource(scGeoHandle);
    }
}