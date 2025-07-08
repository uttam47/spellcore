#include <core/Mesh.h>
#include <GL/glew.h>

//TODO: Still figuring out how to design the Mesh class so that it can wrap a RenderCommand inside it. 
//Question to be Answered:
//1. The mesh data loading should happen internally of Mesh class, or it should be done using external Resource Loader?
//   *Incling to the Idea of adding a Spellcore::Resource to interface with Spellcore::ResourceLoader, where each Resource such as Mesh, Shader, Texture could derive from it.
//2 Do I need dynamic Mesh(Resource) update if in future we load things from a Spellcore Specific editor?
namespace AnalyticalApproach::Spellcore
{

}