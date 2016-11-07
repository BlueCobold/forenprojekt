
#include "ShaderContext.hpp"
#include "../Utility.hpp"

double ShaderContext::onGetValueOf(const std::string& name) const
{
    auto found = m_variables.find(name);
    if(found != end(m_variables))
        return found->second;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("NoVariable"), name));
    return 0;
}

void ShaderContext::onSetValueOf(const std::string& name, const double value)
{
    m_variables[name] = value;
}

void ShaderContext::update()
{
    updateCurrentTime(m_clock.getElapsedTime().asSeconds());
}
