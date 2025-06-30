#include <OpenGL/OpenGLShaderProgram.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <Logger.h>

namespace AnalyticalApproach::Spellcore
{
    const std::unordered_map<ShaderType, std::string> OpenGLShaderProgram::_shaderTypeNames ={
                                                                            {ShaderType::Vertex, "VERTEX"},
                                                                            {ShaderType::Fragment, "FRAGMENT"},
                                                                            {ShaderType::Geometry, "GEOMETRY"},
                                                                            {ShaderType::TessControl, "TESS_CONTROL"},
                                                                            {ShaderType::TessEvaluation, "TESSE_VALUATION"},
                                                                            {ShaderType::Compute, "COMPUTE"}
                                                                        };

    void OpenGLShaderProgram::Attach()
    {
        glUseProgram(_programHandle);
    }

    void OpenGLShaderProgram::Detach()
    {
        glUseProgram(0);
    }

    void OpenGLShaderProgram::Destroy()
    {
        if (_programHandle != 0)
        {
            glDeleteProgram(_programHandle);
            _programHandle = 0;
        }
    }

    GLenum OpenGLShaderProgram::GetShaderType(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::TessControl:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TessEvaluation:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderType::Compute:
            return GL_COMPUTE_SHADER; 
        }
        return GL_VERTEX_SHADER; 
    }

    void OpenGLShaderProgram::Compile()
    {
        if (_shaderStageSources.empty())
            throw std::runtime_error("No shader sources to compile");

        std::vector<GLuint> compiledShaders;

        for (const std::pair<ShaderType, std::string>& pair: _shaderStageSources)
        {
            GLuint shader = glCreateShader(GetShaderType(pair.first));
            const char *src = pair.second.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                std::string message = "OpenGL "+_shaderTypeNames.at(pair.first)+" Shader compilation error: " + std::string(infoLog); 
                LOG_ERROR(message); 
            }

            compiledShaders.push_back(shader);
        }

        _programHandle = glCreateProgram();
        for (GLuint shader : compiledShaders)
            glAttachShader(_programHandle, shader);

        glLinkProgram(_programHandle);

        GLint linkStatus;
        glGetProgramiv(_programHandle, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            char infoLog[512];
            glGetProgramInfoLog(_programHandle, 512, nullptr, infoLog);
            std::string message = "OpenGL Shader linking error: " + std::string(infoLog); 
            LOG_ERROR(message); 
        }

        for (GLuint shader : compiledShaders)
        {
            glDetachShader(_programHandle, shader);
            glDeleteShader(shader);
        }
    }

    void OpenGLShaderProgram::AddStageSource(ShaderType stage, const std::string &source)
    {
        if(!source.empty())
        {
            _shaderStageSources[stage] = source;
        }
    }

    void OpenGLShaderProgram::SetShaderProgramHandle(unsigned int handle)
    {
        _programHandle = handle;
    }

    unsigned int OpenGLShaderProgram::GetShaderProgramHandle() const
    {
        return _programHandle;
    }
}
