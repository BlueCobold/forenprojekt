#pragma once

#ifndef SHADER_PARAMETER_HPP
#define SHADER_PARAMETER_HPP

#include <string>

class ShaderParameter
{
public:
    ShaderParameter(std::string name);

    virtual void bind() = 0;

protected:
    unsigned int getParamLocation();

    const std::string& getName() const;

private:
    std::string m_name;
    int m_shaderProgram;
    int m_paramLocation;
    bool m_paramLocationLoaded;
};

#endif // SHADER_PARAMETER_HPP