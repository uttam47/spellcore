#include <core/ShaderDataType.h>
#include <core/SpellcoreRenderGraph.h>
#include <core/RenderingBackend.h>
#include <RHI/IRenderTargetmanager.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreRenderTargetManager
    {
        std::unordered_map<SCRenderTargetHandle, SCRenderTargetDesc> _renderTargets;
        IRenderTargetManager* _backendRenderTargetManager; 
    public:
        SpellcoreRenderTargetManager(); 
        SCRenderTargetHandle CreateRenderTarget(const SCRenderTargetDesc& renderTargetDesc);
        bool DestroyRenderTarget(SCRenderTargetHandle h);
        const SCRenderTargetDesc& GetRenderTargetDesc(const SCRenderTargetHandle& handle) const;
    };
}