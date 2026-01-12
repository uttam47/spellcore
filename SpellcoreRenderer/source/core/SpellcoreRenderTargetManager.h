#include <core/SpellcoreRenderDataTypes.h>
#include <core/SpellcoreRenderGraph.h>
#include <core/RenderingBackend.h>
#include <RHI/IRenderTargetmanager.h>

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