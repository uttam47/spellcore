#pragma once

#include <core/RenderingSurfaceCreateInfo.h>
#include <core/RenderPipeline.h>
#include <RHI/IRenderingContext.h>
#include <core/Mesh.h>
#include <core/SpellcoreShader.h>

namespace AnalyticalApproach::Spellcore
{
	class SpellcoreRenderer
	{
	private:
		static IRenderingContext* s_RenderingContext;
		static RenderPipeline* s_RenderPipeline;
		static RenderQueue* s_RenderQueue; 

	public:

		/// <summary>
		/// Create a rendering Context with given Rendering Surface Information. 
		/// </summary>
		/// <param name="surfaceInfo"></param>
		/// <returns></returns>
		static bool Initialize(const RenderingSurfaceCreateInfo& surfaceInfo);

		/// <devnote>
		/// 1. These two methods should communicate via shader handles, rather than the internal SpellcoreShader object. 
		/// 2. Loading of Shader can be divided in two paths, CPU and GPU side.
		///  CPU side: 
		///      i. loading of shader file from the disk. 
		///      ii. Compilation/Translation of shader file conent into a SpellcoreShader
		///  GPU side: Compilation and Linking of shader, and retrieving the shader program handle. 
		/// </devnote>
		static SpellcoreShader* LoadShader(std::string shaderPath);
		static void UseShader(SpellcoreShader*);

		/// <summary>
		/// Submits a mesh to the rendering backend for inclusion in the current rendering pass. 
		/// </summary>
		/// <param name="mesh">
		/// Pointer to the mesh object to be submitted. Must remain valid the rendering pass consumes it.
		/// </param>
		static void SubmitMesh(Mesh* mesh);

		/// <summary>
		/// To excluded a Mesh from the current rendering pass, without clearing the whole Render Command.  
		/// </summary>
		/// <param name="mesh">
		/// Pointer to the mesh object to be submitted. Must remain valid the rendering pass consumes it.
		/// </param>
		static void RemoveMesh(Mesh* mesh);

		/// <devnote>
		/// Clear states set for last Rendering pass?!
		/// Still figuring out if should be used for
		///		i.  Defining type of Rendering pass as well. 
		///		ii. Binding render targets. 
		///		iii. Maybe should also have a default SpellcoreShader* parameter as well. 
		/// </devnote>
		static void BeginFrame();

		/// <summary>
		/// It accumulates RenderCommands for all the Mesh"s" submitted to the renderer and flushes to GPU.
		/// </summary>
		static void EndFrame();

		/// <summary>
		/// 
		/// </summary>
		static void Shutdown();
	};

}