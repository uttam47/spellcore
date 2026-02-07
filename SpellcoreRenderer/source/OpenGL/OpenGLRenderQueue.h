#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <RHI/RendereQueue.h>
#include <core/components/SpellcoreRenderDataTypes.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderQueue : public RenderQueue
    {
    private: 
        std::vector<RenderCommand> _commands; 
    public:
        void Submit(const RenderCommand &renderCommand) override;
        void Execute() override;
        void Clear() override; 
        void Sort() override; 
    };
}