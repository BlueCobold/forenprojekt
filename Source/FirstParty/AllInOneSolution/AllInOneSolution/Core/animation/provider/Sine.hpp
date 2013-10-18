#pragma once

#ifndef SINE_HPP
#define SINE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"
#include "../Utility.hpp"

#include <memory>
#include <cmath>

/// Returns the sine of the value of the passed provider
class Sine : public SingleProvider
{
public:
    
    Sine(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue() override
    {
        return sinf(utility::toRadian<float, float>(getProvider()->getValue()));
    }

    virtual Sine* clone() const override
    {
        return new Sine(std::unique_ptr<ValueProvider>(getProvider()->clone()));
    }
};

#endif //SINE_HPP
