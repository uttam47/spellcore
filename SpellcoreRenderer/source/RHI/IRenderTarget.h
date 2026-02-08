#pragma once
#include <core/components/SCRenderTarget.h>

namespace AnalyticalApproach::Spellcore
{
    class IRenderTarget
    {
    public: 
        virtual ~IRenderTarget(); 
        virtual void Bind() = 0; 
        virtual void Unbind() = 0; 
        virtual bool IsValid() = 0; 
        virtual void AddRenderAttachment(const SCImageHandle& imageHandle, const SCRenderImageDesc& renderImageDesc) = 0;
        virtual void AddRenderAttachment(const SCImageHandle& imageHandle, const SCTextureImageDesc& textureImageDesc) = 0;
        virtual void RemoveAttachment(const SCImageHandle& imageHnadle) = 0; 
        virtual SCRenderTargetHandle GetRenderTargetHandle() = 0;
    }; 
} // namespace AnalyticalApproach::Spellcore
