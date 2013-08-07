#pragma once

#ifndef FLOAT_TO_INT_HPP
#define FLOAT_TO_INT_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the int-part of the value of the passed provider
class FloatToInt : public SingleProvider
{
public:
    
    FloatToInt(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue() override
    {
        return static_cast<float>(static_cast<int>(getProvider()->getValue()));
    }
};

#endif //FLOAT_TO_INT_HPP
