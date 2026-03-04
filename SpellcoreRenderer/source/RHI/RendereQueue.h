#pragma once
#include <stdint.h>
#include <core/components/SpellcoreRenderDataTypes.h>
#include <core/systems/SpellcoreRenderGraph.h>

namespace AnalyticalApproach::Spellcore
{
    struct RenderCommand
    {
        enum class DrawType : uint8_t
        {
            INDEXED,
            NON_INDEXED,
            INSTANCED
        };

        SCDataType indexType;
        // Core draw info
        DrawType drawType;
        SCPrimitive primitive;

        uint64_t sortKey = 0;

        // Resource handles (not API-specific)
        uint32_t pipelineID = 0;       // Abstract shader + render state
        uint32_t geometryId = 0;           // VAO/VBO abstraction
        uint32_t materialID = 0;       // Optional (for shader resource sets)
        uint32_t instanceBufferID = 0; // Optional (for instanced rendering)
        uint32_t indexBufferID = 0; // For index Buffer

        // Draw range
        uint32_t elementCount = 0; // Number of indices/vertices
        uint32_t baseVertex = 0;
        uint32_t firstIndex = 0; // Byte offset / element index
        uint32_t instanceCount = 1;
        uint32_t firstInstance = 0;

        // Optional bindable texture set
        uint32_t textureIDs[16] = {0}; // Fixed-size array for cache locality
        uint8_t textureCount = 0;

        // Optional metadata
        uint32_t objectID = 0; // UBO offset / per-object buffer index
    };

    class RenderQueue
    {
    public:
        virtual void Submit(const RenderCommand &renderCommand) = 0;
        virtual void Execute() = 0;
        virtual void Clear() = 0;
        virtual void Sort() = 0; 
        virtual ~RenderQueue() = default;
    };
}