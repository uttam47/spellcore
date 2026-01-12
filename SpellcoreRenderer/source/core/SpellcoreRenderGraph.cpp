#include <core/SpellcoreRenderGraph.h>

namespace AnalyticalApproach::Spellcore
{
    bool SpellcoreRenderGraph::AddPass(const SCRenderPassNode& scrpNode)
    {
        return false; 
    }

    bool SpellcoreRenderGraph::Resolve()
    {
        return false; 

    }

    const std::vector<SCRenderPassHandle> SpellcoreRenderGraph::GetRenderPassExecutionOrder() const
    {
        return _exectionOrder; 
    }
}