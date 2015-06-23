
#include "IntParameter.hpp"
#include "../GLExt.hpp"

IntParameter::IntParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider) :
        ShaderParameter(name),
        m_provider(std::move(provider))
{ }

void IntParameter::onBind(unsigned int paramLocation)
{
    gl::Uniform1i(paramLocation, static_cast<int>(m_provider->getValue()));
}
