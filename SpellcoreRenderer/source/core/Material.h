#pragma once
#include <core/SpellcoreShader.h>
#include <RHI/ITexture.h>

namespace AnalyticalApproach::Spellcore
{
    class Material
    {
        SpellcoreShader* _shader;
        std::vector<ITexture *> textures;
    };
}