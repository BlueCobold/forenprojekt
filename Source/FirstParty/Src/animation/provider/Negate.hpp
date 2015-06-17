#pragma once

#ifndef NEGATE_HPP
#define NEGATE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>

/// Returns the negated value of the passed provider (-1 * provider)
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

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<Negate>(new Negate(getProvider()->clone()));
    }
};

#endif //NEGATE_HPP
