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
        RGBA8,
        RGBA16F,
        D24S8,
        D32F
    };

    enum class SCRTAttachmentType : uint8_t
    {
        Color = 0, 
        Depth, 
        Stencil
    };

    struct SpellcoreRenderImage
    {
        bool shaderReadBack = false; 
        SCRTAttachmentType attachmentType = SCRTAttachmentType::Color; 
        SCRTFormat  renderFormat = SCRTFormat::RGBA16F;

        //Only used for colors
        uint8_t attachmentIndex = 0; 
    };

    struct SCRTDescription
    {
        uint32_t width = 0;
        uint32_t height = 0;

        std::string name = "RenderTarget";
        std::vector<SpellcoreRenderImage> scImages; 
    };

    struct SCRenderPassNode
    {
        std::string name;

        // "reads" means sampled input of RT color (v0 rule)
        std::vector<SCRTHandle> reads;

        // "writes" means this pass binds RT as framebuffer output
        std::vector<SCRTHandle> writes;
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
