#pragma once
#include <vector>
#include <core/Material.h>
#include <RHI/IndexBuffer.h>
#include <RHI/VertexArray.h>
#include <RHI/RendereQueue.h>

//#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    class Submesh
    {
    private: 
        RenderCommand _renderCommand; 
        
        uint64_t GenerateSortKey(); 
    public:
        GeometryBuffer *vertexArray;
        IndexBuffer *indexBuffer;
        Material *material;
        uint32_t indexCount;

        void Refresh(); 
        const RenderCommand& GetRenderCommand(); 
    };

    class Mesh
    {
    public:
        std::vector<Submesh*> submeshes;
    };

}
