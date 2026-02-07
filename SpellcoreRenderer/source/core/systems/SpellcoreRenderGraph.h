#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{

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
