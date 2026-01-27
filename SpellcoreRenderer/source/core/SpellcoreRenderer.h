#pragma once

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
		static RenderQueue* s_RenderQueue; 

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

		static uint32_t UploadGeometry(const SCGeometryData& scGeometryData);
		static bool UpdateGeometry(const SCGeometryHandle& scGeoHandle, const SCGeometryData& scGeoemtryData);
		static void ReleaseGeometry(SCGeometryHandle& scGeoHandle);
	};

}