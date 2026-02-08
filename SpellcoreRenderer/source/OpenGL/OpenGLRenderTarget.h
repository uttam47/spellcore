#pragma once
#include <RHI\IRenderTarget.h>
#include <OpenGL/OpenGLRenderBufferObject.h>
#include <OpenGL/OpenGLTextureImage.h>
#include <unordered_map>

namespace AnalyticalApproach::Spellcore
{
	class OpenGLRenderTarget : public IRenderTarget
	{
	private: 

		struct Attachment
		{
			bool   isRenderbuffer;    
			SCRTAttachmentType attachment; 
			uint32_t colorIndex = 0;
			SCImageHandle handle;            
		};


	public:
		OpenGLRenderTarget();
		~OpenGLRenderTarget(); 
		void Bind() override;
		void Unbind() override;
		bool IsValid() override; 

		void AddRenderAttachment(const SCImageHandle& imageHandle, const SCRenderImageDesc& renderImageDesc) override;
		void AddRenderAttachment(const SCImageHandle& imageHandle, const SCTextureImageDesc& textureImageDesc) override;
		void RemoveAttachment(const SCImageHandle& imageHnadle) override; 
		SCRenderTargetHandle GetRenderTargetHandle() override; 


	private: 

		bool _isValid = false;
		SCRTDescription scrtDescription; 
		SCRenderTargetHandle _fboHandle = 0;
		
		void UpdateDrawBuffers();
		void ValidateFramebufferComplete();

		std::unordered_map<SCImageHandle, Attachment> _bindings; 


	};
} // namespace AnalyticalApproach
