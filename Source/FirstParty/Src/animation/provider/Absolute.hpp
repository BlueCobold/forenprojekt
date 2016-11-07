#pragma once

#ifndef ABSOLUTE_HPP
#define ABSOLUTE_HPP

#include "SingleProvider.hpp"

#include <memory>
#include <cmath>

/// Returns the absolute value of the passed provider
class Absolute : public SingleProvider
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Absolute>(new Absolute(getProvider()->clone()));
    }

    double calculateValue() override
    {
        return fabs(getProvider()->getValue());
    }

public:
    Absolute(std::unique_ptr<ValueProvider> provider) : SingleProvider(std::move(provider))
    { }
};

#endif //ABSOLUTE_HPP
