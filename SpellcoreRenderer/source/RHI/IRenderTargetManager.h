#pragma once
#include <core/SpellcoreRenderGraph.h>

namespace AnalyticalApproach::Spellcore
{
	class IRenderTargetManager
	{
	public: 
		virtual SCRTHandle CreateRenderTarget(const SCRTDescription& scrtDescription) = 0;
		virtual bool DestroyRenderTarget(SCRTHandle& scrtHandle) = 0;
		virtual bool UseRenderTarget(const SCRTHandle& scrtHandle) = 0; 
	};
}
