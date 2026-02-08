#pragma once
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/components/SCRenderTarget.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLRenderBufferObject
    {
        SCImageHandle _rboHandle;

    public:
        OpenGLRenderBufferObject(const SCRenderImageDesc& renderImgDescription);
        ~OpenGLRenderBufferObject();
        SCImageHandle GetRBOHandle(); 
    }; 

} 
