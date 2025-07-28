#pragma once
#include <vector>
#include <core/Material.h>
#include <RHI/GeometryBuffer.h>
#include <RHI/RendereQueue.h>

namespace AnalyticalApproach::Spellcore
{
    class Submesh
    {
    private: 
        RenderCommand _renderCommand; 
        
        uint64_t GenerateSortKey(); 
    public:
        GeometryBuffer *vertexArray;
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
