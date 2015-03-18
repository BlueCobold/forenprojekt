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
public:

    Pow(std::vector<std::unique_ptr<ValueProvider>>&& provider) : MultiProvider(std::move(provider))
    {
       if(getProvider().size() != 2)
           throw std::runtime_error(utility::replace(utility::translateKey("TwoChilds"), "Pow"));
    }

    virtual float getValue() override
    {
        float base = getProvider()[0]->getValue();
        float exponent = getProvider()[1]->getValue();
        return powf(base, exponent);
    }

    virtual Pow* clone() const override
    {
        auto list = cloneProviders();
        return new Pow(std::move(list));
    }
};

#endif //POW_HPP
