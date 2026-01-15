#pragma once

#include <unordered_map>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/components/SCGeometryData.cpp>
#include <core/SpellcoreRenderingBackend.h>


namespace AnalyticalApproach::Spellcore
{
    //TODO: Move these data description in their specific files. 

    enum class SCPixelFormat : uint8_t
    {
        Unknown = 0,

        // 8-bit normalized integer formats (most common for images)
        R8,
        RG8,
        RGB8,
        RGBA8,

        // 16-bit integer formats
        R16,
        RG16,
        RGB16,
        RGBA16,

        // 16-bit float formats (HDR pipelines)
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        // 32-bit float formats (rare, heavy)
        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        // Special / utility
        BGRA8,      // common on Windows image loaders
    };

    enum class SCTextureFilters : uint8_t
    {
        Nearest = 0,
        Linear,

        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class SCTextureWrap : uint8_t
    {
        Repeat = 0,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    struct SCImageData
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8; // distinct from render formats if you want
        std::vector<uint8_t> bytes;
    };


    struct SCTextureDesc
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8; // includes SRGB variants if needed
        bool generateMips = true;

        // sampler-ish
        SCTextureFilters minFilter = SCTextureFilters::LinearMipmapLinear;
        SCTextureFilters magFilter = SCTextureFilters::Linear;
        SCTextureWrap wrapU = SCTextureWrap::Repeat;
        SCTextureWrap wrapV = SCTextureWrap::Repeat;
    };



    class SCRendererResourceManager
    {

    private:

        IRendererBackendFactory* _renderingBackend;

        std::unordered_map<SCGeometryHandle, GeometryBuffer*> _geometryBuffers; 
        std::unordered_map<SCShaderHandle, SpellcoreShader*> _shaders; 
        std::unordered_map < SCTextureHandle, uint32_t> _textures; 
       
    public:

        SCRendererResourceManager(); 
        //TODO: Check whether it's possible to pass this as const or not, keeping it as it for now. 
        SCGeometryHandle CreateSpellcoreGeometry(const SCGeometryData& meshData);
        bool UpdateSpellcoreGeometry(const SCGeometryData& meshData);
        bool DestroySpellcoreGeometry(SCGeometryHandle& scMeshHandle);

        SCMaterialHandle CreateSpellcoreMaterial(); 

        SCTextureHandle CreateTexture(const SCTextureDesc& scTextureDesc);
        SCTextureHandle CreateTexture(const SCTextureDesc& scTextureDesc, const SCImageData* initialData);
        bool UpdateTexture(SCTextureHandle scTextureHandle, const SCImageData* imageData);

        //TODO: Pull Bifurcate Render Pipeline and Shader Manager into two different entities. 
        SCShaderHandle CreateSpellcoreShader();
    };
}


