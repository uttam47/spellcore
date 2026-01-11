#pragma once
#include <core/SpellcoreShader.h>

namespace AnalyticalApproach::Spellcore
{
	class Material
	{
		SpellcoreShader* _shader;

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