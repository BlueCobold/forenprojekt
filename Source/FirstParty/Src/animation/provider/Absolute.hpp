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

    virtual Absolute* clone() const override
    {
        return new Absolute(std::unique_ptr<ValueProvider>(getProvider()->clone()));
    }
};

#endif //ABSOLUTE_HPP
