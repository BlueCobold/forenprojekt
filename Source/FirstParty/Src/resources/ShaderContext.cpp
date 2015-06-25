
#include "ShaderContext.hpp"
#include "../Utility.hpp"

float ShaderContext::getValueOf(const std::string& name) const
{
    auto found = m_variables.find(name);
    if(found != end(m_variables))
        return found->second;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("NoVariable"), name));
    return 0;
}

void ShaderContext::setValueOf(const std::string& name, const float value)
{
    m_variables[name] = value;
}

void ShaderContext::update()
{
    updateCurrentTime(m_clock.getElapsedTime().asSeconds());
}
