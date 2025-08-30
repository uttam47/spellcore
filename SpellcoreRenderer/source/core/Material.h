#pragma once
#include <core/SpellcoreShader.h>
#include <RHI/ITexture.h>

namespace AnalyticalApproach::Spellcore
{
	class Material
	{
		SpellcoreShader* _shader;
		std::vector<ITexture*> textures;

	public:

		Material(SpellcoreShader* shader)
		{
			_shader = shader;
		}

		SpellcoreShader* GetShader()
		{
			return _shader;
		}

		std::vector<uint32_t> GetTextureIds()
		{
			return {  };
		}
	
		uint32_t GetShaderHandle()
		{
			return _shader->GetShaderHandle(); 
		}
		


	};
}