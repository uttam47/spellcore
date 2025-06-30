#pragma once
#include <stdint.h>

namespace AnalyticalApproach::Spellcore
{
    /*
        If you're using ECS or scene graphs, each object could carry a RenderPassMask to declare which passes it's relevant for.

        Consider using a bitmask or range instead of a strict enum, if you want to make passes dynamic (e.g., custom material passes).

        Some engines support custom render passes defined by materials or render graphs — you could extend the enum later to include UserDefined.
    */

    // TODO: Remove this, we will go with bit masks for render pass.
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

    struct RenderCommand
    {
        uint64_t sortKey;  // Packed sorting key
        uint32_t shaderID; // OpenGL program ID
        uint32_t vaoID;    // OpenGL VAO
        uint32_t indexCount;
        uint32_t materialID;
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