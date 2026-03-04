#pragma once
#include <Logger.h>

namespace AnalyticalApproach::Spellcore
{
    class IRendererBackendFactory; 
    enum class GraphicsApi
    {
        OpenGL,
        Vulkan,
        DirectX12
    };

    class SpellcoreRenderingBackend
    {
        static IRendererBackendFactory *_instance;

    public:
        static void Initialize(GraphicsApi api); 
        static IRendererBackendFactory* Get(); 
        static void Shutdown();
    };
}