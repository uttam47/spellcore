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
        uint64_t GenerateSortKey(); 
        RenderCommand _renderCommand; 
    public:
        GeometryBuffer *geometryBuffer;
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
