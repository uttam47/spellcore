#include <RHI/IRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
	class OpenGLRenderTargetManager: public IRenderTargetManager
	{
	public:

		SCRTHandle CreateRenderTarget(const SCRTDescription& scrtDescription) override;
		bool DestroyRenderTarget(SCRTHandle& scrtHandle) override;
		bool UseRenderTarget(const SCRTHandle& scrtHandle) override; 
	};
}