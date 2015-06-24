#pragma once

#ifndef SHADER_PARAMETER_HPP
#define SHADER_PARAMETER_HPP

class DrawParameter;

#include <string>

class ShaderParameter
{
public:
    ShaderParameter(const std::string& name);

    void bind(int shaderProgram);

    virtual void prepare(const DrawParameter& param) = 0;

protected:
    virtual void onBind(int paramLocation) = 0;
    const std::string& getName() const;
    int getParamLocation(int shaderProgram);

private:
    std::string m_name;
    int m_paramLocation;
    bool m_paramLocationLoaded;
};

#endif // SHADER_PARAMETER_HPP
