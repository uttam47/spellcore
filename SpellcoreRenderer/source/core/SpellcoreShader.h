#pragma once

#include <core/BufferLayout.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreShader
    {
        private:

            BufferLayout _layout;
            uint32_t _shaderHandle; 
            std::string _shaderSource; 

        public:

            SpellcoreShader(std::string shaderPath); 
            ~SpellcoreShader();

            const std::string& GetSourcePath(); 
            
            void SetBufferLayout(BufferLayout BufferLayout); 
            const BufferLayout& GetBufferLayout(); 
            const std::string& GetShaderSource(); 

            const std::string& GetSource() {return _shaderSource; }

            void SetShaderHandle(uint32_t shaderHandle); 
            uint32_t GetShaderHandle(); 
    }; 
}

/*
No....

Let's first go over how thinks will work?


________________________
Data Composition:
________________________

Mesh is Composed of
    vector<submeshs>
    vector<Materials>

Submesh is Composed
    VertexArray
    IndexArray
    VertexBuffer or vector<vertexBuffers>

Material is Composed of
    vector<textures>
    IShader

SpellcoreRenderer (Static) 
    creates: 
        IShaderCompiler
        IRenderPipeline
    Manages
        unordered_map<RenderPassType, CommandQueue<Mesh>> //Should be IDrawCommand or Mesh works fine?
_______________________________
Now for the steps how it goes:
_______________________________

----Main Loop Start---
SpellcoreRenderer::GetInstance()->BeginFrame(); 

//Such multiple commands
SpellcoreRenderer:GetInatnsce()->Submit(mesh_1); 
SpellcoreRenderer:GetInatnsce()->Submit(mesh_2); 
SpellcoreRenderer:GetInatnsce()->Submit(mesh...); 
SpellcoreRenderer:GetInatnsce()->Submit(mesh...); 
SpellcoreRenderer:GetInatnsce()->Submit(mesh_n);  

Spellcore::GetInstance()->EndFrame(); 
----Main Loop End---
_______________________________

How it works internally:

The Submit method: Internally determines which CommandQueue the mesh belongs to and append to it, or else create a new one.
The EndFrame: 

    Responsible for 
        Sorts Mesh in a way that GPU communication (GPU Specific calls) can reduced? 
            Sorting Priority-> Materials (ShaderProgram > Textures used) (Can you give me a more comprehnsive or shade more light upon how it's handle by greatly optimized handle)
        Clearing each CommandQueue, also responsible for binding objects like Frame Buffer Object( I hope each API like OpenGL has such object in them which we renderer to, so that it can be abstracted)


One thing, I want you to remember:
My Goal isn't to render a triangle on Screen, but to create a well desinged rendering architecture. So whenever you think, I am derailing from that kind of structure do let me know. 
As for Rendering actual stuff, we will eventually get something on screen.
So tell me now, as per what I have laid out using our discussion. This is how my Renderer looks. Anything you would like to add, or is it going good? 

I just have superficial Knowledge how Sorting the Mesh with Shader and Textures (or some other parameters) increases the performance. But I don't have actual hands on experience. 
Tell me how it looks what can be improved, what's going right. 
*/