#pragma once

#include <core/RenderingSurfaceCreateInfo.h>
#include <core/RenderPipeline.h>
#include <RHI/IRenderingContext.h>
#include <core/Mesh.h>
#include <core/SpellcoreShader.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreRenderer
    {
    private:
        static IRenderingContext *s_RenderingContext;
        static RenderPipeline *s_RenderPipeline; 
    
    public:
        static bool Initialize(const RenderingSurfaceCreateInfo &surfaceInfo);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();
    };

}