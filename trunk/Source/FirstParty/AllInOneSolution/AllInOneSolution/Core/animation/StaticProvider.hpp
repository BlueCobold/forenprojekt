#pragma once

#ifndef STATIC_PROVIDER_HPP
#define STATIC_PROVIDER_HPP

/// Returns a static value
class StaticProvider : public ValueProvider
{
private:

    float m_value;

public:
    StaticProvider(const float value) : m_value(value)
    {
    }

    virtual float getValue() const
    {
        return m_value;
    }
};

#endif //STATIC_PROVIDER_HPP
