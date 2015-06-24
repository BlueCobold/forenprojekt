
#include "ShaderParameter.hpp"
#include "../DrawParameter.hpp"
#include "../GLExt.hpp"

ShaderParameter::ShaderParameter(const std::string& name) :
        m_paramLocation(-1),
        m_paramLocationLoaded(false),
        m_name(name)
{ }

void ShaderParameter::bind(int shaderProgram)
{
    auto location = getParamLocation(shaderProgram);
    if(location < 0)
        return;
    onBind(location);
}

int ShaderParameter::getParamLocation(int shaderProgram)
{
#ifndef NO_SHADER
    if(m_paramLocationLoaded)
        return m_paramLocation;
        
    m_paramLocationLoaded = true;
    m_paramLocation = gl::GetUniformLocation(shaderProgram, m_name.c_str());
#endif
    return m_paramLocation;
}

const std::string& ShaderParameter::getName() const
{
    return m_name;
}
