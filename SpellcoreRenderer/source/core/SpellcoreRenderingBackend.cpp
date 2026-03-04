#include <stdexcept>
#include <OpenGL/OpenGLObjectFactory.h>
#include <core/SpellcoreRenderingBackend.h>

namespace AnalyticalApproach::Spellcore
{

	IRendererBackendFactory* SpellcoreRenderingBackend::_instance = nullptr;

	void SpellcoreRenderingBackend::Initialize(GraphicsApi api)
	{
		switch (api)
		{
		case GraphicsApi::OpenGL:
			//_instance = new OpenGLFactory();
			_instance = new OpenGLObjectFactory();
			break;
		case GraphicsApi::Vulkan:
			break;
		case GraphicsApi::DirectX12:
			break;
		default:
			LOG_ERROR("Unsupported graphics API");
			break;
		}
	}

	IRendererBackendFactory* SpellcoreRenderingBackend::Get()
	{
		if (_instance  == nullptr)
		{
			LOG_ERROR("Spellcore's Rendering backend is not initialized");
			throw std::runtime_error(" Rendering backend not initialized");
		}

		return _instance;
	}

	void SpellcoreRenderingBackend::Shutdown()
	{
		delete _instance;
		_instance = nullptr;
	}
}