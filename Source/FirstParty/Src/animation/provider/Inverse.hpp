#pragma once

#ifndef INVERSE_HPP
#define INVERSE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the inverse (1/x) of the value of the passed provider
class Inverse : public SingleProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Inverse>(new Inverse(getProvider()->clone()));
    }
    
    double calculateValue() override
    {
        auto value = getProvider()->getValue();
        if((value >= FLT_EPSILON) || (value <= -FLT_EPSILON))
            return 1/value;
        else
            return 0;
    }

public:
    Inverse(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }
};

#endif //INVERSE_HPP
