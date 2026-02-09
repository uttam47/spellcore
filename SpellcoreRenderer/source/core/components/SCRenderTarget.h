#pragma once

#include <vector>
#include <string>
#include <optional>   
#include <cstdint>

#include "SpellcoreRenderDataTypes.h"

namespace AnalyticalApproach::Spellcore
{
    struct SCImageData
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8;
        std::vector<uint8_t> bytes;
    };

    // Storage description (backend-agnostic)
    struct SCImageDesc
    {
        SCImageType type = SCImageType::Tex2D;

        uint32_t width = 0;
        uint32_t height = 0;

        // For Tex3D: depth > 1
        uint32_t depth = 1;

        // For arrays/cubes: layers > 1 (cube must be multiple of 6)
        uint32_t layers = 1;

        SCRTFormat format = SCRTFormat::RGBA16F;
        SCImageUsage usage = SCImageUsage::ColorAttachment;
        SCRTSampleCount sampleCount = SCRTSampleCount::x1;
        SCMipLevels mipLevels = SCMipLevels::Auto;
    };

    // Sampler description (separate entity; maps well to VkSampler)
    struct SCSamplerDesc
    {
        SCTextureFilters minFilter = SCTextureFilters::LinearMipmapLinear;
        SCTextureFilters magFilter = SCTextureFilters::Linear;
        SCTextureWrap wrapU = SCTextureWrap::ClampToEdge;
        SCTextureWrap wrapV = SCTextureWrap::ClampToEdge;
        bool compareEnable = false;
    };

    // What the pass wants conceptually
    struct SCAttachmentDesc
    {
        SCRTAttachmentType attachmentType = SCRTAttachmentType::Color;
        uint8_t attachmentIndex = 0;  // only for Color
        SCImageDesc image;

        bool generateMips = false;                // if the output should have mip chain generated after rendering
        std::optional<SCSamplerDesc> sampler;     // if it will be sampled later
    };

    struct SCRTDescription
    {
        uint32_t width = 0, height = 0;
        std::string name = "RenderTarget";
        std::vector<SCAttachmentDesc> attachments;
    };

    // Slot key (attachment identity, independent of handle)
    struct SCAttachmentSlot
    {
        SCRTAttachmentType type = SCRTAttachmentType::Color;
        uint32_t index = 0; // Color index; ignored for Depth/Stencil
    };

    // View/subresource selection for attaching (Vulkan-style concept)
    struct SCImageViewDesc
    {
        uint32_t mipLevel = 0;

        // For array/cube: layer index (face = layer for cube)
        uint32_t layer = 0;

        // For 3D: slice (z) index, if you model 3D as slices
        uint32_t slice = 0;

        // If true: attach as "layered" (all layers), enabling layered rendering
        bool layered = false;
    };

    struct SCRTAttachmentBinding
    {
        SCAttachmentSlot slot;
        SCImageHandle imageHandle = 0;
        SCImageViewDesc view{};
    };

} // namespace AnalyticalApproach::Spellcore