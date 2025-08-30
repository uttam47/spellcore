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
        Material *_material;
    public:
        Submesh(MeshData* meshData, Material* material); 
        uint32_t indexCount;
        const RenderCommand& GetRenderCommand(); 

        void Refresh(); 
        void SetMaterial(Material* material); 

        ~Submesh(); 
    };

    class Mesh
    {
    public:
        std::vector<Submesh*> submeshes;
    };

}
