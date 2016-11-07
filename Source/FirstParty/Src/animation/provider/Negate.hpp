#pragma once

#ifndef NEGATE_HPP
#define NEGATE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"

#include <memory>

/// Returns the negated value of the passed provider (-1 * provider)
class Negate : public SingleProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Negate>(new Negate(getProvider()->clone()));
    }
    
    double calculateValue() override
    {
        auto value = getProvider()->getValue();
        return -1 * value;
    }

public:
    Negate(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }
};

#endif //NEGATE_HPP
