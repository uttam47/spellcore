#pragma once
#include <vector>
#include <core/Material.h>
#include <RHI/GeometryBuffer.h>
#include <RHI/RendereQueue.h>
#include <core/MeshData.h>

namespace AnalyticalApproach::Spellcore
{
    class Submesh
    {
    private: 
        MeshData* _meshData;
        uint64_t GenerateSortKey(); 
        RenderCommand _renderCommand; 
        GeometryBuffer *_geometryBuffer = nullptr;
    public:
        Submesh(MeshData* meshData); 
        Material *material;
        uint32_t indexCount;

        void Refresh(); 
        const RenderCommand& GetRenderCommand(); 

        ~Submesh(); 
    };

    class Mesh
    {
    public:
        std::vector<Submesh*> submeshes;
    };

}
