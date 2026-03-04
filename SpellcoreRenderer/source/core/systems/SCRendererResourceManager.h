#pragma once

#include <unordered_map>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{
    class ITexture; 
    class IRendererBackendFactory; 
    class SCRenderTarget; 
    class SCRTDescription; 
    class SCGeometryData; 
    class SCImageDesc; 
    class SpellcoreShader; 
    class GeometryBuffer; 
    class SCImageData; 

    class SCRendererResourceManager
    {

    private:

        static SCRendererResourceManager* STATIC_INSTANCE;
        SCRendererResourceManager(); 
        ~SCRendererResourceManager(); 

        IRendererBackendFactory* _renderingBackendFactory; 

        std::unordered_map<SCTextureHandle, ITexture*> _textures;
        std::unordered_map<SCShaderHandle, SpellcoreShader*> _shaders; 
        std::unordered_map<SCGeometryHandle, GeometryBuffer*> _geometryBuffers; 
        std::unordered_map<SCRenderTargetHandle, SCRTDescription> _renderTargets;
       
    public:

        static SCRendererResourceManager* GetInstance();

        // TODO: Check whether it's possible to pass this as const or not, keeping it as it for now.
        SCGeometryHandle CreateGeometryResource(const SCGeometryData& meshData); 
        bool UpdateGeometryResource(const SCGeometryHandle& scGeoHandle, const SCGeometryData& meshData); 
        bool DestroyGeometryResource(SCGeometryHandle& scGeoHandle); 

        SCImageHandle CreateTexture(const SCImageDesc& scTextureDesc);
        SCImageHandle CreateTexture(const SCImageDesc& scTextureDesc, const SCImageData* initialData);
        bool UpdateTexture(SCImageHandle scTextureHandle, const SCImageData* imageData);

        SCRenderTargetHandle CreateRenderTarget(const SCRTDescription& renderTargetDesc); 
        const SCRTDescription* GetRenderTargetDesc(const SCRenderTargetHandle& handle) const; 
        bool DestroyRenderTarget(SCRenderTargetHandle h); 
        bool UseRenderTarget(const SCRenderTargetHandle& h); 

        // TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities.
        SCMaterialHandle CreateSpellcoreMaterial(); 
        SCShaderHandle CreateSpellcoreShader(); 

        static void DestroyInstance();
    };

    using RenderResourceRegistry = SCRendererResourceManager; 
}


