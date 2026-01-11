#include <RHI/IRenderTargetManager.h>

namespace AnalyticalApproach::Spellcore
{
	class OpenGLRenderTargetManager: public IRenderTargetManager
	{
	public:

		SCRenderTargetHandle CreateRenderTarget(const SCRenderTargetDesc& renderTargetDesc) override;
		bool DestroyRenderTarget(const SCRenderTargetHandle& renderTargetHandle) override;
	};
}