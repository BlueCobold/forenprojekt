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
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<FloatToInt>(new FloatToInt(getProvider()->clone()));
    }
    
    float calculateValue() override
    {
        return static_cast<float>(static_cast<int>(getProvider()->getValue()));
    }

public:
    FloatToInt(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }
};

#endif //FLOAT_TO_INT_HPP
