#include "SpellcoreRenderTargetManager.h"

namespace AnalyticalApproach::Spellcore
{
	SpellcoreRenderTargetManager::SpellcoreRenderTargetManager()
	{
		_backendRenderTargetManager = RenderingBackend::Get()->CreateRenderTargetManager(); 
	}

	SpellcoreRenderTargetManager::~SpellcoreRenderTargetManager()
	{
		if (_backendRenderTargetManager)
		{
			delete _backendRenderTargetManager; 
		}
	}

	SCRenderTargetHandle SpellcoreRenderTargetManager::CreateRenderTarget(const SCRenderTargetDesc& renderTargetDesc)
	{
		auto scrtHandle = _backendRenderTargetManager->CreateRenderTarget(renderTargetDesc); 
		_renderTargets[scrtHandle] = renderTargetDesc; 
		return scrtHandle; 
	}

	bool SpellcoreRenderTargetManager::DestroyRenderTarget(SCRenderTargetHandle handle)
	{
		auto it = _renderTargets.find(handle);
		
		if (it != _renderTargets.end())
		{
			_backendRenderTargetManager->DestroyRenderTarget(handle); 
			_renderTargets.erase(it); 
			return true; 
		}
		
		return false; 
	}

	const SCRenderTargetDesc& SpellcoreRenderTargetManager::GetRenderTargetDesc(const SCRenderTargetHandle& handle) const
	{
		auto it = _renderTargets.find(handle); 
		if (it != _renderTargets.end())
		{
			return it->second;
		} 

		//TODO: Add assertion for invalid access. 
	}

}