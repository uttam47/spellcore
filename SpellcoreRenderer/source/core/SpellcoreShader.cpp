#include <core/SpellcoreShader.h>
#include <Utils/FileReader.h>

namespace AnalyticalApproach::Spellcore
{

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
    }

    const std::string& SpellcoreShader::GetShaderSource()
    {
        return _shaderSource; 
    }

    void SpellcoreShader::SetBufferLayout(BufferLayout BufferLayout)
    {
    }
    const BufferLayout &SpellcoreShader::GetBufferLayout()
    {
    }

    void SpellcoreShader::SetShaderHandle(uint32_t shaderHandle)
    {
    }
    uint32_t SpellcoreShader::GetShaderHandle()
    {
    }

}