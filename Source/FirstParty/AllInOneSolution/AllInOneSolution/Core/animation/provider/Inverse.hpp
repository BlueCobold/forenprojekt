#pragma once

#ifndef INVERSE_HPP
#define INVERSE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"
#include "../Utility.hpp"

#include <memory>
#include <cmath>

/// Returns the inverse (1/x) of the value of the passed provider
class Inverse : public SingleProvider
{
public:
    
    Inverse(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue() override
    {
        float value = getProvider()->getValue();
        if((value >= FLT_EPSILON) || (value <= -FLT_EPSILON))
            return 1/value;
        else
            return 0;
    }

    virtual Inverse* clone() const override
    {
        return new Inverse(std::unique_ptr<ValueProvider>(getProvider()->clone()));
    }
};

#endif //INVERSE_HPP
