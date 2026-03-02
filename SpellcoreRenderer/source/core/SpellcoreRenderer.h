#pragma once

#include <unordered_map>

#include <core/SpellcoreShader.h>
#include <RHI/IRenderingContext.h>
#include <core/components/SCGeometryData.h>
#include <core/systems/SpellcoreRenderGraph.h> 
#include <core/systems/SpellcoreRenderPipeline.h>
#include <core/components/RenderingSurfaceCreateInfo.h>
#include <core/systems/SCRendererResourceManager.h>

namespace AnalyticalApproach::Spellcore
{
	class SpellcoreRenderer
	{
	private:
		static IRenderingContext* s_RenderingContext;
		static SpellcoreRenderPipeline* s_RenderPipeline;
		static RenderResourceRegistry* s_RenderResourceRegistry;
		static SpellcoreRenderGraph* s_SCRenderGraph; 


		using RenderPassExeInfo = SCRenderPassNode; 
		struct RenderPass
		{
			RenderPassExeInfo exeInfo;
			RenderQueue* queue; 
		};

		static std::unordered_map< SCRenderPassHandle, RenderPass> _renderQeues;


	public:

		//Change Rendering Surface info to something better. 
		static bool Initialize(const RenderingSurfaceCreateInfo& surfaceInfo);

		static SpellcoreShader* LoadShader(std::string shaderPath);
		static void UseShader(SpellcoreShader*);
		static void BeginFrame();
		static void EndFrame();
		static void Shutdown();
		static void RenderFrame(); 
		static void AddRenderPass(const std::string& renderPassName); 
		static void SubmitMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPassKey);
		static void RemoveMesh(const SCGeometryHandle& scGeoHandle, const std::string& renderPasskey);
	};

}