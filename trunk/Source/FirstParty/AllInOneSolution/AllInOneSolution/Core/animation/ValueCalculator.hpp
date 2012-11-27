#pragma once

#ifndef VALUE_CALCULATOR_HPP
#define VALUE_CALCULATOR_HPP

#include "ValueProvider.hpp"

#include <memory> // unique_ptr

/// Returns a specific calculated value calculated from the provider's value.
class ValueCalculator
{
private:

    std::unique_ptr<ValueProvider> m_provider;

public:

    ValueCalculator(std::unique_ptr<ValueProvider> provider)
        : m_provider(std::move(provider))
    {
    }

    virtual float calculateValue() const
    {
        return m_provider->getValue();
    }

protected:

    const ValueProvider* getProvider() const
    {
        return m_provider.get();
    }
};

#endif // VALUE_CALCULATOR_HPP
