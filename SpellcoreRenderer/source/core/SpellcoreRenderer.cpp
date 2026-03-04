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


	std::unordered_map< SCRenderPassHandle, SpellcoreRenderer::RenderPass> SpellcoreRenderer::_renderQeues;

	bool SpellcoreRenderer::Initialize(const RenderingSurfaceCreateInfo& surfaceInfo)
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
	
	}

	void SpellcoreRenderer::AddRenderPass(const SCRenderPassHandle& scrpHandle, const RenderPass& renderPass)
	{
		
	}

	void SpellcoreRenderer::RemoveRenderPass(const SCRenderPassHandle& scrpHandle)
	{

	}

	void SpellcoreRenderer::SubmitRenderCommand(const RenderCommand& renderCommand, const SCRenderPassHandle& scrpHandle)
	{

		auto it = _renderQeues.find(scrpHandle);
		if (it != _renderQeues.end())
		{
			_renderQeues[scrpHandle].queue->Submit(renderCommand); 
		}
		else
		{
			RenderQueue* renderQueue = SpellcoreRenderingBackend::Get()->CreateRenderQueue();
			RenderPass renderPass;

			renderPass.queue = renderQueue;

			_renderQeues[scrpHandle] = renderPass;

			renderQueue->Submit(renderCommand);
		}

	}

}