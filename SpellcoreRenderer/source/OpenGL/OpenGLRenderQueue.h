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
        std::unordered_map<SCRenderPassHandle, std::vector<RenderCommand>> _renderPassBuckets; 
    public:
        void Submit(const SCRenderPassHandle& scrpHandle, const RenderCommand &renderCommand) override;
        void Execute(const SCRenderPassHandle& scrpHandle) override;
        void Clear() override; 

        GLenum ToGLPrimitiveType(SCPrimitive primitive);
        GLenum ToGLIndexType(SCDataType shaderIndexType); 

    };
}