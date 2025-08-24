#include <core/SpellcoreShader.h>
#include "FileUtils/FileReader.h"

namespace AnalyticalApproach::Spellcore
{

    //NOTE: Does SpellcoreShader even need the path info, shouldn't it just get the created using shader content itself? 
    SpellcoreShader::SpellcoreShader(std::string shaderPath)
    {
        FileSystem::FileReader shaderFileReader(shaderPath); 
        _shaderSource = shaderFileReader.getContent(); 
    }

    SpellcoreShader::~SpellcoreShader()
    {
    }

    const std::string &SpellcoreShader::GetSourcePath()
    {
        return ""; 
    }

    const std::string& SpellcoreShader::GetShaderSource()
    {
        return _shaderSource; 
    }

    void SpellcoreShader::SetBufferLayout(GPUBufferLayout GPUBufferLayout)
    {
    }

    const GPUBufferLayout& SpellcoreShader::GetBufferLayout()
    {
        return _layout; 
    }

    void SpellcoreShader::SetShaderHandle(uint32_t shaderHandle)
    {
    }

    uint32_t SpellcoreShader::GetShaderHandle()
    {
        return 0; 
    }

}