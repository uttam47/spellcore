#pragma once
#include <RHI/IRenderResourceManager.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderResourceManager: public IRenderResourceManager
    {
    private:
        /* data */
    public:
        OpenGLRenderResourceManager(/* args */);
        ~OpenGLRenderResourceManager();


        // TODO: Check whether it's possible to pass this as const or not, keeping it as it for now.
         SCGeometryHandle CreateGeometryResource(const SCGeometryData &meshData)  override; 
         bool UpdateGeometryResource(const SCGeometryHandle &scGeoHandle, const SCGeometryData &meshData)  override; 
         bool DestroyGeometryResource(SCGeometryHandle &scGeoHandle)  override; 


         SCTextureHandle CreateTexture(const SCTextureDesc &scTextureDesc)  override; 
         SCTextureHandle CreateTexture(const SCTextureDesc &scTextureDesc, const SCImageData *initialData)  override; 
         bool UpdateTexture(SCTextureHandle scTextureHandle, const SCImageData *imageData)  override; 

         SCRenderTargetHandle CreateRenderTarget(const SCRTDescription &renderTargetDesc)  override; 
         bool DestroyRenderTarget(SCRenderTargetHandle& h)  override; 
         bool UseRenderTarget(const SCRenderTargetHandle &h)  override; 

        // TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities.
         SCShaderHandle CreateSpellcoreShader()  override;
         SCMaterialHandle CreateSpellcoreMaterial()  override;
    };
 
    
} // namespace AnalyticalApproach::Spellcore
