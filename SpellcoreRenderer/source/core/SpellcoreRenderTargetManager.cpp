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

	SCRTHandle SpellcoreRenderTargetManager::CreateRenderTarget(const SCRTDescription& renderTargetDesc)
	{
		auto scrtHandle = _backendRenderTargetManager->CreateRenderTarget(renderTargetDesc); 
		_renderTargets[scrtHandle] = renderTargetDesc; 
		return scrtHandle; 
	}

	bool SpellcoreRenderTargetManager::DestroyRenderTarget(SCRTHandle handle)
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

	bool SpellcoreRenderTargetManager::UseRenderTarget(const SCRTHandle& scrtHandle)
	{

	}

	const SCRTDescription& SpellcoreRenderTargetManager::GetRenderTargetDesc(const SCRTHandle& handle) const
	{
		auto it = _renderTargets.find(handle); 
		if (it != _renderTargets.end())
		{
			return it->second;
		} 

		//TODO: Add assertion for invalid access. 
	}

}