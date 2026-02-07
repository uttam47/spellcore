#pragma once

#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/components/SCRenderTarget.h>
#include <core/components/SCGeometryData.h>

namespace AnalyticalApproach::Spellcore
{
    class IRenderResourceManager
    {
    public:

        // TODO: Check whether it's possible to pass this as const or not, keeping it as it for now.
        virtual SCGeometryHandle CreateGeometryResource(const SCGeometryData &meshData) = 0;
        virtual bool UpdateGeometryResource(const SCGeometryHandle &scGeoHandle, const SCGeometryData &meshData) = 0;
        virtual bool DestroyGeometryResource(SCGeometryHandle &scGeoHandle) = 0;

        virtual SCTextureHandle CreateTexture(const SCTextureDesc &scTextureDesc) = 0;
        virtual SCTextureHandle CreateTexture(const SCTextureDesc &scTextureDesc, const SCImageData *initialData) = 0;
        virtual bool UpdateTexture(SCTextureHandle scTextureHandle, const SCImageData *imageData) = 0;

        virtual SCRenderTargetHandle CreateRenderTarget(const SCRTDescription &renderTargetDesc) = 0;
        virtual bool DestroyRenderTarget(SCRenderTargetHandle& h) = 0;
        virtual bool UseRenderTarget(const SCRenderTargetHandle &h) = 0;

        // TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities.
        virtual SCShaderHandle CreateSpellcoreShader() = 0;
        virtual SCMaterialHandle CreateSpellcoreMaterial() = 0;
    };

}
