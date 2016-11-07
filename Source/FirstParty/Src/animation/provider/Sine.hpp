#pragma once

#ifndef SINE_HPP
#define SINE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"
#include "../../Utility.hpp"

#include <memory>
#include <cmath>

/// Returns the sine of the value of the passed provider
class Sine : public SingleProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Sine>(new Sine(getProvider()->clone()));
    }

    double calculateValue() override
    {
        return sin(utility::toRadian(getProvider()->getValue()));
    }

public:
    Sine(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }
};

#endif //SINE_HPP
