
#include "IntParameter.hpp"
#include "../DrawParameter.hpp"
#include "../GLExt.hpp"

IntParameter::IntParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider) :
        ShaderParameter(name),
        m_provider(std::move(provider))
{ }

void IntParameter::onBind(int paramLocation)
{
    gl::Uniform1i(paramLocation, static_cast<int>(m_provider->getValue()));
}

void IntParameter::prepare(const DrawParameter& param)
{ }
