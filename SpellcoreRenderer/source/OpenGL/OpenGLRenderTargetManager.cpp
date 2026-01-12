#include <OpenGL/OpenGLRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
	SCRTHandle OpenGLRenderTargetManager::CreateRenderTarget(const SCRTDescription& scrtDescription)
	{
		return 0; 
	}

	bool OpenGLRenderTargetManager::DestroyRenderTarget(SCRTHandle& scrtHandle)
	{
		return false; 
	}

	bool OpenGLRenderTargetManager::UseRenderTarget(const SCRTHandle& scrtHandle)
	{
		return false; 
	}
}