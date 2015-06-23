
#include "FloatParameter.hpp"
#include "../GLExt.hpp"

FloatParameter::FloatParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider) :
        ShaderParameter(name),
        m_provider(std::move(provider))
{ }

void FloatParameter::onBind(unsigned int paramLocation)
{
    gl::Uniform1f(paramLocation, m_provider->getValue());
}
