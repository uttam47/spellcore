#pragma once
#include <stdint.h>
#include <core/ShaderDataType.h>

namespace AnalyticalApproach::Spellcore
{
    /*
        If you're using ECS or scene graphs, each object could carry a RenderPassMask to declare which passes it's relevant for.

        Consider using a bitmask or range instead of a strict enum, if you want to make passes dynamic (e.g., custom material passes).

        Some engines support custom render passes defined by materials or render graphs — you could extend the enum later to include UserDefined.
    */

    // TODO: Remove this, we will go with bit masks for render pass. And this shouldn't be defined here, but inside core not in RHI. 
    enum class RenderPassType
    {
        // --- Depth & Shadow ---
        DepthPrepass,      // Early-Z or pre-depth-only pass (optional, used in AAA engines)
        ShadowDirectional, // Shadow map rendering for directional lights (e.g., sun)
        ShadowPoint,       // Cube map rendering for point light shadows
        ShadowSpot,        // Spot light shadow maps

        // --- G-buffer (Deferred shading) ---
        GBuffer, // Geometry pass: outputs albedo, normals, depth, etc.
        SSAO,    // Screen-space ambient occlusion (optional)

        // --- Lighting ---
        DeferredLighting, // Deferred shading lighting pass
        ForwardLighting,  // Forward shading pass (transparent/alpha objects)
        Transparent,      // Alpha blended geometry rendered after opaque

        // --- Post-processing ---
        ToneMapping, // Convert HDR to LDR and apply exposure curve
        Bloom,       // Light bleeding/glow
        MotionBlur,
        DepthOfField,
        FXAA,         // Anti-aliasing
        ColorGrading, // LUT / post color adjustments
        UIOverlay,    // ImGui, HUD, etc.
        FinalBlit     // Final screen blit or swapchain present
    };

    enum class Primitive : uint8_t
    {
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN, // Optional
        LINES,
        LINE_STRIP,
        LINE_LOOP, // Optional
        POINTS,
        PATCHES
    };

    struct RenderCommand
    {
        enum class DrawType : uint8_t
        {
            INDEXED,
            NON_INDEXED,
            INSTANCED
        };

        ShaderDataType indexType;
        // Core draw info
        DrawType drawType;
        Primitive primitive;

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
        virtual void Submit(RenderPassType renderPass, const RenderCommand &renderCommand) = 0;
        virtual void Execute(RenderPassType renderPass) = 0;
        virtual void Clear() = 0;
        virtual ~RenderQueue() = default;
    };
}