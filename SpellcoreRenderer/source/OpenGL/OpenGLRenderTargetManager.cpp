#include <OpenGL/OpenGLRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
	SCRenderTargetHandle OpenGLRenderTargetManager::CreateRenderTarget(const SCRTDescription& scrtDescription)
	{
		return 0; 
	}

	bool OpenGLRenderTargetManager::DestroyRenderTarget(SCRenderTargetHandle& scrtHandle)
	{
		return false; 
	}

	bool OpenGLRenderTargetManager::UseRenderTarget(const SCRenderTargetHandle& scrtHandle)
	{
		return false; 
	}
}