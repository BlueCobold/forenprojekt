#pragma once

#ifndef ABSOLUTE_HPP
#define ABSOLUTE_HPP

#include "SingleProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the absolute value of the passed provider
class Absolute : public SingleProvider
{
public:
    
    Absolute(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue() override
    {
        return fabsf(getProvider()->getValue());
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<Absolute>(new Absolute(getProvider()->clone()));
    }
};

#endif //ABSOLUTE_HPP
