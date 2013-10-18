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

    virtual float getValue() override
    {
        float value = getProvider()->getValue();
        return -1 * value;
    }

    virtual Negate* clone() const override
    {
        return new Negate(std::unique_ptr<ValueProvider>(getProvider()->clone()));
    }
};

#endif //NEGATE_HPP
