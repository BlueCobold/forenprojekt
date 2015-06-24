#pragma once

#ifndef FLOAT_PARAMETER_HPP
#define FLOAT_PARAMETER_HPP

#include "ShaderParameter.hpp"
#include "../../animation/provider/ValueProvider.hpp"

#include <memory>

class FloatParameter : public ShaderParameter
{
public:
    FloatParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider);

    virtual void onBind(int paramLocation) override;
    virtual void prepare(const DrawParameter& param) override;

private:
    std::unique_ptr<ValueProvider> m_provider;
};

#endif // FLOAT_PARAMETER_HPP
