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
    RenderQueue* SpellcoreRenderer::s_RenderQueue = nullptr;

    bool SpellcoreRenderer::Initialize(const RenderingSurfaceCreateInfo &surfaceInfo)
    {
        RenderingBackend::Initialize(GraphicsApi::OpenGL); 
        
        s_RenderingContext = RenderingBackend::Get()->CreateRenderingContext(); 
        s_RenderQueue = RenderingBackend::Get()->CreateRenderQueue(); 

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
        {
            s_RenderingContext->BeginFrame();
        }
    }

    void SpellcoreRenderer::EndFrame()
    {
        if (s_RenderQueue)
        {
            //TODO: Instead of executing just one type of Render pass, execute them all as per their priority. 
            // Or give the option to do so individually, so that it has the flexibility to Render to any bound target. 
            s_RenderQueue->Execute(RenderPassType::ForwardLighting); 
        }
        if (s_RenderingContext)
        {
            s_RenderingContext->EndFrame();
        }
    }

    void SpellcoreRenderer::SubmitMesh(Mesh* mesh)
    {
        if (s_RenderQueue)
        {
            for (Submesh* submesh: mesh->submeshes)
            {
                //TODO: Render Pass type should also be present in the Mesh as an parameter. 
                s_RenderQueue->Submit(RenderPassType::ForwardLighting, submesh->GetRenderCommand()); 
            }
        }
    }

    void SpellcoreRenderer::RemoveMesh(Mesh* mesh)
    {
        if (s_RenderQueue)
        {
            for (Submesh* submesh : mesh->submeshes)
            {
                //TODO: to remove all traversed Submesh's rendercommand from the current s_RenderQueue.
            }
        }
    }
}