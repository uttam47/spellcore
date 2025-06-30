#pragma once

#include <string> 

namespace AnalyticalApproach::Spellcore
{    
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        TessControl,
        TessEvaluation,
        Compute
    }; 

    class IShaderProgram
    {
    public:
        virtual ~IShaderProgram();

        virtual void Attach() = 0;
        virtual void Detach() = 0;
        virtual void Destroy() = 0;
        virtual void Compile() = 0; 

        virtual void AddStageSource(ShaderType stage, const std::string &source) = 0; 

        virtual void SetShaderProgramHandle(unsigned int handle) = 0;
        virtual unsigned int GetShaderProgramHandle() const = 0;
    };

}