#pragma once

#ifndef STATIC_PROVIDER_HPP
#define STATIC_PROVIDER_HPP

#include "ValueProvider.hpp"

/// Returns a static value
class StaticProvider : public ValueProvider
{
    float m_value;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<StaticProvider>(new StaticProvider(m_value));
    }

public:
    StaticProvider(const float value) : m_value(value)
    {
    }

    float getValue() override
    {
        return m_value;
    }
};

#endif //STATIC_PROVIDER_HPP
