#pragma once

#include <vector>
#include <string>
#include "SpellcoreRenderDataTypes.h"

namespace AnalyticalApproach::Spellcore
{
    //For CPU Readback. 
    struct SCImageData
    {
        uint32_t width = 0, height = 0;
        SCPixelFormat format = SCPixelFormat::RGBA8; // distinct from render formats if you want
        std::vector<uint8_t> bytes;
    };

    struct SCRenderImageDesc
    {
        uint32_t width = 0, height = 0;
        SCRTAttachmentType attachmentType = SCRTAttachmentType::Color; 
        SCRTFormat  renderFormat = SCRTFormat::RGBA16F;
        SCImageUsage usage = SCImageUsage::ColorAttachment;// | SCImageUsage::Sampled;
        SCRTSampleCount sampleCount; 
        uint8_t attachmentIndex = 0; 
    };

    struct SCTextureImageDesc: SCRenderImageDesc
    {
        bool generateMips = true;
        SCTextureFilters minFilter = SCTextureFilters::LinearMipmapLinear;
        SCTextureFilters magFilter = SCTextureFilters::Linear;
        SCTextureWrap wrapU = SCTextureWrap::Repeat;
        SCTextureWrap wrapV = SCTextureWrap::Repeat;
        SCMipLevels mipLevels = SCMipLevels::Auto; 
    };

    struct SCRTDescription
    {
        uint32_t width = 0;
        uint32_t height = 0;

        std::string name = "RenderTarget";
        std::vector<SCRenderImageDesc> scImages; 
    };
    
} // namespace AnalyticalApproach::Spellcore
