#pragma once

#ifndef RAMP_PROVIDER_HPP
#define RAMP_PROVIDER_HPP

#include "SingleProvider.hpp"

/// If the given value is less than min, it returns 0
/// If the given value is above max, it returns 1
/// If the given value is inbetween, it returns (val-min)/(max-min)
/// visually it returns:
///   ^
/// 1_|          ______
///   |         /
///   |        / 
///   |       /  
///   |      /   
///  0|_____/_________
///         |    |
///        min  max
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
        {
            if(value < m_min)
                return 0;
            else
                return 1;
        }
        return (value - m_min) / diff;
    }

    virtual Ramp* clone() const override
    {
        return new Ramp(m_min, m_max, std::unique_ptr<ValueProvider>(getProvider()->clone()));
    }
};

#endif //RAMP_PROVIDER_HPP
