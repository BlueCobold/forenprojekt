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

    void prepare(const DrawParameter& param);

protected:
    const std::string& getName() const;
    int getParamLocation(int shaderProgram);

private:
    std::string m_name;
    int m_paramLocation;
    bool m_paramLocationLoaded;
    
    virtual void onBind(int paramLocation) = 0;
    virtual void onPrepare(const DrawParameter& param);
};

#endif // SHADER_PARAMETER_HPP
