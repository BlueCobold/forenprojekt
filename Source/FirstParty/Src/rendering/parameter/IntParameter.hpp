#pragma once

#ifndef INT_PARAMETER_HPP
#define INT_PARAMETER_HPP

#include "ShaderParameter.hpp"
#include "../../animation/provider/ValueProvider.hpp"

#include <memory>

class IntParameter : public ShaderParameter
{
public:
    IntParameter(const std::string& name, std::unique_ptr<ValueProvider>&& provider);

    virtual void onBind(int paramLocation) override;
    virtual void prepare(const DrawParameter& param) override;

private:
    std::unique_ptr<ValueProvider> m_provider;
};

#endif // FLOAT_PARAMETER_HPP
