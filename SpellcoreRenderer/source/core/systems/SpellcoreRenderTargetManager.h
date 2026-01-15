#include <RHI/IRenderTargetmanager.h>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/systems/SpellcoreRenderGraph.h>
#include <core/SpellcoreRenderingBackend.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreRenderTargetManager
    {
        std::unordered_map<SCRenderTargetHandle, SCRTDescription> _renderTargets;
        IRenderTargetManager* _backendRenderTargetManager; 
    public:
        SpellcoreRenderTargetManager();
        ~SpellcoreRenderTargetManager(); 

        SCRenderTargetHandle CreateRenderTarget(const SCRTDescription& renderTargetDesc);
        const SCRTDescription& GetRenderTargetDesc(const SCRenderTargetHandle& handle) const;
        bool DestroyRenderTarget(SCRenderTargetHandle h);
        bool UseRenderTarget(const SCRenderTargetHandle& h); 
    };
}