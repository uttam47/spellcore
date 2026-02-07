#pragma once

#include <vector>
#include <string>
#include "SpellcoreRenderDataTypes.h"

namespace AnalyticalApproach::Spellcore
{
    struct SCRenderImageDesc
    {
        SCRTAttachmentType attachmentType = SCRTAttachmentType::Color; 
        SCRTFormat  renderFormat = SCRTFormat::RGBA16F;
        SCImageUsage usage = SCImageUsage::ColorAttachment;// | SCImageUsage::Sampled;
        SCRTSampleCount sampleCount; 

        //Only used for colors
        uint8_t attachmentIndex = 0; 
    };

    struct SCRTDescription
    {
        uint32_t width = 0;
        uint32_t height = 0;

        std::string name = "RenderTarget";
        std::vector<SCRenderImageDesc> scImages; 
    };

    struct SCImageData
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8; // distinct from render formats if you want
        std::vector<uint8_t> bytes;
    };

    struct SCTextureDesc
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8; // includes SRGB variants if needed
        bool generateMips = true;

        // sampler-ish
        SCTextureFilters minFilter = SCTextureFilters::LinearMipmapLinear;
        SCTextureFilters magFilter = SCTextureFilters::Linear;
        SCTextureWrap wrapU = SCTextureWrap::Repeat;
        SCTextureWrap wrapV = SCTextureWrap::Repeat;
    };

} // namespace AnalyticalApproach::Spellcore
