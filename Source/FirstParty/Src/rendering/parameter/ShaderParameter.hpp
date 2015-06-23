#pragma once

#ifndef SHADER_PARAMETER_HPP
#define SHADER_PARAMETER_HPP

#include <string>

class ShaderParameter
{
public:
    ShaderParameter(const std::string& name);

    void bind();

protected:
    virtual void onBind(unsigned int paramLocation) = 0;

    unsigned int getParamLocation();
    const std::string& getName() const;

private:
    std::string m_name;
    int m_shaderProgram;
    int m_paramLocation;
    bool m_paramLocationLoaded;
};

#endif // SHADER_PARAMETER_HPP
