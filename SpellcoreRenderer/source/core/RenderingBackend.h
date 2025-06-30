#pragma once
#include <Logger.h>
#include <RHI/IRendererBackendFactory.h>
#include <OpenGL/OpenGLObjectFactory.h>

namespace AnalyticalApproach::Spellcore
{
    enum class GraphicsApi
    {
        OpenGL,
        Vulkan,
        DirectX12
    };

    class RenderingBackend
    {
        inline static IRendererBackendFactory *_instance = nullptr;

    public:
        static void Initialize(GraphicsApi api)
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

        static IRendererBackendFactory *Get() { return _instance; }

        static void Shutdown()
        {
            delete _instance;
            _instance = nullptr;
        }
    };
}