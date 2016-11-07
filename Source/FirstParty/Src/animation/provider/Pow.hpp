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
    
    double calculateValue() override
    {
        auto base = getProvider()[0]->getValue();
        auto exponent = getProvider()[1]->getValue();
        return pow(base, exponent);
    }

public:
    Pow(std::vector<std::unique_ptr<ValueProvider>> provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() != 2)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Pow"));
    }
};

#endif //POW_HPP
