#include <core/Mesh.h>
#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    void Submesh::Draw()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}