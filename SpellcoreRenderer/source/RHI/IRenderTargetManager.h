#pragma once
#include <core/systems/SpellcoreRenderGraph.h>

namespace AnalyticalApproach::Spellcore
{
	class IRenderTargetManager
	{
	public: 
		virtual SCRenderTargetHandle CreateRenderTarget(const SCRTDescription& scrtDescription) = 0;
		virtual bool DestroyRenderTarget(SCRenderTargetHandle& scrtHandle) = 0;
		virtual bool UseRenderTarget(const SCRenderTargetHandle& scrtHandle) = 0; 
	};
}
