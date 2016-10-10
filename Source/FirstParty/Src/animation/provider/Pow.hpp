#pragma once

#ifndef POW_HPP
#define POW_HPP

#include "MultiProvider.hpp"
#include "ValueProvider.hpp"

#include <cmath>
#include <exception>
#include <memory>

/// Returns the provider 1 ^ provider 2
class Pow : public MultiProvider
{

    std::unique_ptr<ValueProvider> doClone() const override
    {
        auto list = cloneProviders();
        return std::unique_ptr<Pow>(new Pow(std::move(list)));
    }

public:

    Pow(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() != 2)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Pow"));
    }

    float getValue() override
    {
        float base = getProvider()[0]->getValue();
        float exponent = getProvider()[1]->getValue();
        return powf(base, exponent);
    }
};

#endif //POW_HPP
