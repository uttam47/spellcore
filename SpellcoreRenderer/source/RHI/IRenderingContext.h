#pragma once
#include <windows.h>
#include "core/RenderingSurfaceCreateInfo.h"

namespace AnalyticalApproach::Spellcore
{
    class IRenderingContext
    {
        public: 
        virtual bool Initialize(const RenderingSurfaceCreateInfo& renderSurfaceInfo ) = 0; 
        virtual bool Shutdown() =0; 
        virtual bool BeginFrame() =0; 
        virtual bool EndFrame() =0; 
        virtual ~IRenderingContext(); 
    }; 
}