#pragma once

#include <RHI/IShaderProgram.h>
#include <unordered_map>
#include <stdint.h>
#include <GL/glew.h>

namespace AnalyticalApproach::Spellcore
{
    class OpenGLShaderProgram : public IShaderProgram
    {
        std::unordered_map<ShaderType, std::string> _shaderStageSources;

        static const std::unordered_map<ShaderType, std::string> _shaderTypeNames; 
        unsigned int _programHandle;

        GLenum GetShaderType(ShaderType shaderType);

    public:
        void Attach() override;
        void Detach() override;
        void Destroy() override;
        void Compile() override;

        void AddStageSource(ShaderType stage, const std::string &source) override;

        void SetShaderProgramHandle(unsigned int handle) override;
        unsigned int GetShaderProgramHandle() const override;
    };
}