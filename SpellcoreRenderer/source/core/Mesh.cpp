#include <core/Mesh.h>
#include <GL/glew.h>

//TODO: Still figuring out how to design the Mesh class so that it can wrap a RenderCommand inside it. 
//Question to be Answered:
//1. The mesh data loading should happen internally of Mesh class, or it should be done using external Resource Loader?
//   *Incling to the Idea of adding a Spellcore::Resource to interface with Spellcore::ResourceLoader, where each Resource such as Mesh, Shader, Texture could derive from it.
//2 Do I need dynamic Mesh(Resource) update if in future we load things from a Spellcore Specific editor?
namespace AnalyticalApproach::Spellcore
{
    void Submesh::Refresh()
    {
        _renderCommand.pipelineID = material->GetShader()->GetShaderHandle(); 

        int count = 0; 
        for(const uint32_t tId:material->GetTextureIds() )
        {
            _renderCommand.textureIDs[count++] = tId; 
        }

        _renderCommand.geometryId = geometryBuffer->GetId(); 
        _renderCommand.elementCount = geometryBuffer->GetElementCount(); 
        _renderCommand.sortKey = GenerateSortKey(); 
    }

    const RenderCommand& Submesh::GetRenderCommand()
    {
        return _renderCommand; 
    }

    uint64_t Submesh::GenerateSortKey()
    {
        // Example layout (64-bit):
        // [ 8 bits layer | 16 bits shader | 16 bits material | 24 bits depth ]

        // Need to figure this out... 
    
        return 0; 
    }

}