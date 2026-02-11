#pragma once
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/components/SCRenderTarget.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLTextureImage
    {
        SCImageHandle _textureImageHandle;

    public:
        OpenGLTextureImage(const SCImageDesc& textureImageDesc);
        ~OpenGLTextureImage();
        SCImageHandle GetTextureHandle(); 
    }; 
} 
