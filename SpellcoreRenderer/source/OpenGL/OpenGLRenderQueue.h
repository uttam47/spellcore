#include <RHI/RendereQueue.h>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <core/SpellcoreRenderDataTypes.h>

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

        GLenum ToGLPrimitiveType(Primitive primitive);
        GLenum ToGLIndexType(ShaderDataType shaderIndexType); 

    };
}