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
    double m_min;
    double m_max;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Ramp>(new Ramp(m_min, m_max, getProvider()->clone()));
    }

    double calculateValue() override
    {
        auto value = getProvider()->getValue();
        if(value < m_min)
            return 0;
        if(value > m_max)
            return 1;
        auto diff = m_max - m_min;
        if(diff < FLT_EPSILON)
        {
            if(value < m_min)
                return 0;
            else
                return 1;
        }
        return (value - m_min) / diff;
    }

public:
    Ramp(const double min, const double max, std::unique_ptr<ValueProvider> provider) :
        SingleProvider(std::move(provider)),
        m_min(min),
        m_max(max)
    { }
};

#endif //RAMP_PROVIDER_HPP
