#pragma once

#ifndef RAMP_PROVIDER_HPP
#define RAMP_PROVIDER_HPP

#include "SingleProvider.hpp"

/// If the given value is less than min, it returns 0
/// If the given value is above max, it returns 1
/// If the given value is inbetween, it returns (val-min)/(max-min)
class Ramp : public SingleProvider
{
private:

    float m_min;
    float m_max;

public:

    Ramp(const float min, const float max, std::unique_ptr<ValueProvider> provider) :
        SingleProvider(std::move(provider)),
        m_min(min),
        m_max(max)
    { }

    virtual float getValue() override
    {
        float value = getProvider()->getValue();
        if(value < m_min)
            return 0;
        if(value > m_max)
            return 1;
        float diff = m_max - m_min;
        if(diff < FLT_EPSILON)
            return 0;
        return (value - m_min) / (diff);
    }
};

#endif //RAMP_PROVIDER_HPP
