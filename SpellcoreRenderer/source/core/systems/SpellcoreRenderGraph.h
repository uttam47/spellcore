#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/systems/SCRendererResourceManager.h>

namespace AnalyticalApproach::Spellcore
{

    //FOR LATER DEFINING DEPENDCY RESOLUTION MORE CLEARLY. 

    //enum class SCLoadOp { Load, Clear, DontCare };
    //enum class SCStoreOp { Store, DontCare };

    //struct SCPassAttachmentOps
    //{
    //    // keyed by attachment slot (color0, color1, depth) or by image handle
    //    std::vector<SCLoadOp> colorLoad;
    //    std::vector<SCStoreOp> colorStore;

    //    SCLoadOp depthLoad = SCLoadOp::Load;
    //    SCStoreOp depthStore = SCStoreOp::Store;

    //    // optional clear values
    //};
    //struct SCRenderPassNode
    //{
    //    SCRenderTargetHandle output;
    //    SCPassAttachmentOps ops;              // <-- pass intent, not resource intent
    //    std::vector<SCResourceHandle> inputs; // sampled/storage/etc (or keep v0)
    //    std::string name;
    //};

    struct SCRenderPassNode
    {
        SCRenderTargetHandle ouput;
        std::string name;
        std::vector<SCRenderTargetHandle> inputs;
    };

    class SpellcoreRenderGraph
    {
        std::vector<SCRenderPassHandle> _exectionOrder;

        SCRendererResourceManager* _renderResourceRegistry; 

    
    public:
        
        SpellcoreRenderGraph(); 

        bool AddPass(const SCRenderPassNode& scrpNode); 
        bool Resolve(); 
        const std::vector<SCRenderPassHandle> GetRenderPassExecutionOrder() const; 
    };
}
