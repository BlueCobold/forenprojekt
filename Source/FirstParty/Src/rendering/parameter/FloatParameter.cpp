
#include "FloatParameter.hpp"
#include "../DrawParameter.hpp"
#include "../GLExt.hpp"

FloatParameter::FloatParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider) :
        ShaderParameter(name),
        m_provider(std::move(provider))
{ }

void FloatParameter::onBind(int paramLocation)
{
    gl::Uniform1f(paramLocation, static_cast<float>(m_provider->getValue()));
}
