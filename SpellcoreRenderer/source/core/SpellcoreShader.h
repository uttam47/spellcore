#pragma once

#include <core/GPUBufferLayout.h>

namespace AnalyticalApproach::Spellcore
{
    class SpellcoreShader
    {
        private:

            GPUBufferLayout _layout;
            uint32_t _shaderHandle; 
            std::string _shaderSource; 

        public:

            SpellcoreShader(std::string shaderPath); 
            ~SpellcoreShader();

            const std::string& GetSourcePath(); 
            
            void SetBufferLayout(GPUBufferLayout GPUBufferLayout); 
            const GPUBufferLayout& GetBufferLayout(); 
            const std::string& GetShaderSource(); 

            const std::string& GetSource() {return _shaderSource; }

            void SetShaderHandle(uint32_t shaderHandle); 
            uint32_t GetShaderHandle(); 
    }; 
}
