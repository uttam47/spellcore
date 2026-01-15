#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{
    enum class SCRTFormat : uint8_t
    {
        Unknown = 0,

        // 8-bit normalized color formats
        R8_UNorm,
        RG8_UNorm,
        RGB8_UNorm,
        RGBA8_UNorm,

        // 8-bit sRGB formats (shader-visible gamma correction)
        RGB8_sRGB,
        RGBA8_sRGB,

        // 16-bit normalized
        R16_UNorm,
        RG16_UNorm,
        RGB16_UNorm,
        RGBA16_UNorm,

        // 16-bit float (HDR / lighting)
        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        // 32-bit float (G-buffer, compute, storage images)
        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        // Depth / stencil
        D16,
        D24S8,
        D32F,
    };

    enum class SCRTAttachmentType : uint8_t
    {
        Color = 0, 
        Depth, 
        Stencil
    };

    enum class SCRTSampleCount : uint8_t
    {
        x1, 
        x2, 
        x4, 
        x8, 
        x16
    };

    enum class SCImageUsage : uint32_t
    {
        ColorAttachment = 1 << 0,
        DepthStencil = 1 << 1,
        Sampled = 1 << 2,
        Storage = 1 << 3,
        CopySrc = 1 << 4,
        CopyDst = 1 << 5,
        None
    };

    struct SCRenderImageDesc
    {
        SCRTAttachmentType attachmentType = SCRTAttachmentType::Color; 
        SCRTFormat  renderFormat = SCRTFormat::RGBA16F;
        SCImageUsage usage = SCImageUsage::ColorAttachment;// | SCImageUsage::Sampled;
        SCRTSampleCount sampleCount; 

        //Only used for colors
        uint8_t attachmentIndex = 0; 
    };

    struct SCRTDescription
    {
        uint32_t width = 0;
        uint32_t height = 0;

        std::string name = "RenderTarget";
        std::vector<SCRenderImageDesc> scImages; 
    };

    struct SCRenderPassNode
    {
        std::string name;

        // "reads" means sampled input of RT color (v0 rule)
        std::vector<SCRenderTargetHandle> reads;

        // "writes" means this pass binds RT as framebuffer output
        std::vector<SCRenderTargetHandle> writes;
    };

    class SpellcoreRenderGraph
    {
        std::vector<SCRenderPassHandle> _exectionOrder;
    
    public:
        
        bool AddPass(const SCRenderPassNode& scrpNode); 
        bool Resolve(); 
        const std::vector<SCRenderPassHandle> GetRenderPassExecutionOrder() const; 
    };
}
