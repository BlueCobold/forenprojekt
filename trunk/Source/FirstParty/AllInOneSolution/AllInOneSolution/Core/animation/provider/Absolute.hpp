#pragma once

#ifndef ABSOLUTE_HPP
#define ABSOLUTE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the absolute value of the passed provider
class Absolute : public SingleProvider
{
public:
    
    Absolute(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue()
    {
        return fabsf(getProvider()->getValue());
    }
};

#endif //ABSOLUTE_HPP
