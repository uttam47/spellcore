#include <RHI/RendereQueue.h>
#include <unordered_map>
#include <vector>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderQueue : public RenderQueue
    {
    private: 
        std::unordered_map<RenderPassType, std::vector<RenderCommand>> _renderPassBuckets; 
    public:
        void Submit(RenderPassType renderPass, const RenderCommand &renderCommand) override;
        void Execute(RenderPassType renderPass) override;
        void Clear() override;
    };
}