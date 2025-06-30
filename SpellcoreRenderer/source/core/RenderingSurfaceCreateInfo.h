#pragma once

namespace AnalyticalApproach::Spellcore
{
   struct RenderingSurfaceCreateInfo
   {
      void* nativeWindowHandle = nullptr; 
      int width = 0; 
      int height = 0; 
      bool vsynchEnabled = 0;  
   }; 
}

/*

Okay so let me elaborate on my doubts: 


We are not supposed to use any kind of virtual calls in the main loop. Not broadly in the main loop, but for hot code path of the main loop. 
Places like rendering, doing physics, or any other core system which requires traversing our whole set of objects. 

Now the question comes in context of rendering how to achieve this, while keeping our Renderer Core api agnostic. 
Where's the struggle for me or I think it to be?

Currently I have 4 critical classes which would be in the hot code path for rendering, and are derving from interfaces (So they have virtual calls). 
The concerend classes listed below: 

IShaderProgram
IVertexBuffer
IVertexArray
ITexture

A Mesh or Submesh is basically composed of these 4 class' objects. 

So when I have to draw Mesh, the wrong way I could do is something like this: 
//Leave aside objects sorting for performant rendering at the moment. Current focus is to reduce the vtable overhead.
//pseudocode
class Mesh
{
public: 
std::vector<Submesh*> _submeshList; 
}

class Submesh
{
public:
IVertexArray* _vertexArray; 
IShaderProgram* _shaderProgram; 
std::vector<ITexture*> _textures;
}
for (submesh in Mesh)
{
   submesh->Draw(); 
}

where submesh's Draw function looks like this: 

void Sumbesh::Draw()
{
   _shaderPrgroam->Use(); 
   _vertexArray->Bind(); //All textures are already bound with this Mesh State 
   _vertexArray->Draw(); //Internally has the buffer size and draws it directly using api specific implemnetation. As Draw would be virtual.
}

But I believe this is not something we want if we wanna save the dynamic overhead?

Now the question comes, how to side step it, or do it better. 
My general guess as of yet is to create a struct, which holds the handles for ShaderProgram, VertexArray(GPU state object for a geometry to be drawn), textures. 
And instead of defining a Draw method in vertexArray class, we culminate this data after initialization of the Mesh, into the said struct. And queue it?

struct RenderTarget
{
   unsigned int handle; //Handle would representng the state object which holds details for buffers and textures to be used. 
   unsigned int programHandle; // Which shader program to use. 
}; 
So now our Submesh class would be something like this; 

class Submesh:
{
   private: 
     RenderTarget _renderTarget; 
   public: 

   
IVertexArray* vertexArray; 
IShaderProgram* shaderProgram; 
std::vector<ITexture*> textures;
 RenderTarget GetRenderTarget()
 {
 return _renderTarget;
 }

 void Init(); //Basically initialize the _renderTarget Object with the necessary information. 
 {
     _renderTarget.programHandle = shaderProgram.GetHandle(); 
     _renderTarget.handle = vertexArray.GetHandle(); 
 }
}


And then we can simply pass the Mesh to the Renderer something like: 

SpellcoreRenderer::Get()->Submit(mesh); 
internally in the mesh we create a queue of all the renderTargets, or sort them as per our convenience, and execute them all, with whatever backend we have no. 
Is this how we can keep it performant or, there's some other way? 

Also I am mostly concerned about, whether we can squeeze all the details necessary to render a mesh into a plain struct line renderTarget for all APIs like opengl, vulkan or D3D12. 

Currently I know that OpenGL has VertexArrayObject which could be used to represent a single mesh to be drawn, including all the buffers(single when data is interleaved, multiple when data is non-interleaved) and textures?
And since it is represented by a plain int it's fine like its rendering pipeline. Defining a renderTarget for OpenGL seems fine, but what about vulkan and d3d12?

I haven't worked with vulkan or d3d12, but I want to be able to support them, whole point of defining RHI, if they provide similar kind of plain int handles to represent a mesh on GPU and rendering piplene that it's great. If not, tell me how usually it happens for them? 

*/