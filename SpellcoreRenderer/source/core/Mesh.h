#pragma once
#include <vector>
#include <core/Material.h>
#include <RHI/IndexBuffer.h>
#include <RHI/VertexArray.h>
//#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    class Submesh
    {
    public:
        VertexArray *vertexArray;
        IndexBuffer *indexBuffer;
        Material *material;
        uint32_t indexCount;
    };

    class Mesh
    {
    public:
        std::vector<Submesh*> submeshes;
    };

}
