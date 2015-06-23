
#include "ShaderParameter.hpp"
#include "../GLExt.hpp"

ShaderParameter::ShaderParameter(const std::string& name) :
        m_paramLocation(-1),
        m_shaderProgram(-1),
        m_paramLocationLoaded(false),
        m_name(name)
{ }

void ShaderParameter::bind()
{
    auto location = getParamLocation();
    if(location < 0)
        return;
    onBind(location);
}

unsigned int ShaderParameter::getParamLocation()
{
#ifndef NO_SHADER
    if(m_paramLocationLoaded)
        return m_paramLocation;
        
    gl::GetIntegerv(gl::CURRENT_PROGRAM, &m_shaderProgram);
    m_paramLocationLoaded = true;
    m_paramLocation = gl::GetUniformLocation(m_shaderProgram, m_name.c_str());
#endif
    return m_paramLocation;
}

const std::string& ShaderParameter::getName() const
{
    return m_name;
}
