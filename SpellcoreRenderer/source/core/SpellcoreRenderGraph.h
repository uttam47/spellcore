#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <core/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{
    enum class SCRTFormat : uint8_t
    {
        Unknown = 0,
        RGBA8,
        RGBA16F,
        D24S8,
        D32F
    };

    enum class SCRTSampleCount : uint8_t
    {
        _1 = 1,
        _2 = 2,
        _4 = 4,
        _8 = 8
    };

    enum class SCRTAccess : uint8_t
    {
        None = 0,
        SampledByShader, // implies texture-backed for GL
    };

    inline bool IsColorFormat(SCRTFormat f)
    {
        return (f == SCRTFormat::RGBA8 || f == SCRTFormat::RGBA16F);
    }

    inline bool IsDepthFormat(SCRTFormat f)
    {
        return (f == SCRTFormat::D24S8 || f == SCRTFormat::D32F);
    }

    struct SCColorAttachmentDesc
    {
        SCRTFormat format = SCRTFormat::RGBA8;
        SCRTAccess access = SCRTAccess::None;
    };

    struct SCDepthAttachmentDesc
    {
        SCRTFormat format = SCRTFormat::D24S8;
    };

    struct SCRenderTargetDesc
    {
        const char *debugName = "RenderTarget";

        uint32_t width = 0;
        uint32_t height = 0;

        SCRTSampleCount samples = SCRTSampleCount::_1;

        bool hasColor = true;
        SCColorAttachmentDesc color = {};

        bool hasDepth = true;
        SCDepthAttachmentDesc depth = {};
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
        std::vector<SCRenderPassNode> _exectionOrder; 
    
    public:
        
        bool AddPass(const SCRenderPassNode& scrpNode); 
        bool Resolve(); 
        bool Execute(); 
    };
}
