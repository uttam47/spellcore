#include <RHI/IRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
	class OpenGLRenderTargetManager: public IRenderTargetManager
	{
	public:

		SCRenderTargetHandle CreateRenderTarget(const SCRTDescription& scrtDescription) override;
		bool DestroyRenderTarget(SCRenderTargetHandle& scrtHandle) override;
		bool UseRenderTarget(const SCRenderTargetHandle& scrtHandle) override; 
	};
}