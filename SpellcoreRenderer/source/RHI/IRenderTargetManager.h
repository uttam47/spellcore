#pragma once
#include <core/SpellcoreRenderGraph.h>

namespace AnalyticalApproach::Spellcore
{
	class IRenderTargetManager
	{
	public: 
		virtual SCRenderTargetHandle CreateRenderTarget(const SCRenderTargetDesc& scrtDesc) = 0;
		virtual bool DestroyRenderTarget(const SCRenderTargetHandle& scrtHandle) = 0;
	};
}
