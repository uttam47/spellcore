#include <OpenGL/OpenGLRenderResourceManager.h>


namespace AnalyticalApproach::Spellcore
{

	OpenGLRenderResourceManager::OpenGLRenderResourceManager(/* args */)
	{
	}

	OpenGLRenderResourceManager::~OpenGLRenderResourceManager()
	{
	}

	// TODO: Check whether it's possible to pass this as const or not, keeping it as it for now.
	SCGeometryHandle OpenGLRenderResourceManager::CreateGeometryResource(const SCGeometryData& meshData)
	{
		return 0; 
	}

	bool OpenGLRenderResourceManager::UpdateGeometryResource(const SCGeometryHandle& scGeoHandle, const SCGeometryData& meshData)
	{
		return false; 
	}

	bool OpenGLRenderResourceManager::DestroyGeometryResource(SCGeometryHandle& scGeoHandle)
	{
		return false; 
	}

	SCTextureHandle OpenGLRenderResourceManager::CreateTexture(const SCTextureDesc& scTextureDesc)
	{
		return 0; 
	}

	SCTextureHandle OpenGLRenderResourceManager::CreateTexture(const SCTextureDesc& scTextureDesc, const SCImageData* initialData)
	{
		return 0; 
	}

	bool OpenGLRenderResourceManager::UpdateTexture(SCTextureHandle scTextureHandle, const SCImageData* imageData)
	{
		return false; 
	}

	SCRenderTargetHandle OpenGLRenderResourceManager::CreateRenderTarget(const SCRTDescription& renderTargetDesc)
	{
		return 0; 
	}

	bool OpenGLRenderResourceManager::DestroyRenderTarget(SCRenderTargetHandle& h)
	{
		return false; 
	}

	bool OpenGLRenderResourceManager::UseRenderTarget(const SCRenderTargetHandle& h)
	{
		return false; 
	}

	// TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities.
	SCShaderHandle OpenGLRenderResourceManager::CreateSpellcoreShader()
	{
		return 0;
	}

	SCMaterialHandle OpenGLRenderResourceManager::CreateSpellcoreMaterial()
	{
		return 0;
	}
}