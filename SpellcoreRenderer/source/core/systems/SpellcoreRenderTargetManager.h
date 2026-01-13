#include <RHI/IRenderTargetmanager.h>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/systems/SpellcoreRenderGraph.h>
#include <core/SpellcoreRenderingBackend.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreRenderTargetManager
    {
        std::unordered_map<SCRTHandle, SCRTDescription> _renderTargets;
        IRenderTargetManager* _backendRenderTargetManager; 
    public:
        SpellcoreRenderTargetManager();
        ~SpellcoreRenderTargetManager(); 

        SCRTHandle CreateRenderTarget(const SCRTDescription& renderTargetDesc);
        const SCRTDescription& GetRenderTargetDesc(const SCRTHandle& handle) const;
        bool DestroyRenderTarget(SCRTHandle h);
        bool UseRenderTarget(const SCRTHandle& h); 
    };
}