#pragma once

#ifndef NEGATE_HPP
#define NEGATE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"
#include "../Utility.hpp"

#include <memory>

/// Returns the inverse (1/x) of the value of the passed provider
class Negate : public SingleProvider
{
public:
    
    Negate(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }

    virtual float getValue()
    {
        float value = getProvider()->getValue();
        return -1 * value;
    }
};

#endif //NEGATE_HPP
