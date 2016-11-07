#pragma once

#ifndef DELAY_PROVIDER_HPP
#define DELAY_PROVIDER_HPP

#include "SingleProvider.hpp"

class Delay : public SingleProvider
{
    double m_start;
    double m_duration;
    
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Delay>(new Delay(m_start, m_duration, getProvider()->clone()));
    }

    double calculateValue() override
    {
        auto value = getProvider()->getValue();
        if(value < m_start)
            return value;
        if(value > m_start + m_duration)
            return value - m_duration;
        return m_start;
    }
    
public:
    Delay(const double start, const double duration, std::unique_ptr<ValueProvider> provider) :
        SingleProvider(std::move(provider)),
        m_start(start),
        m_duration(duration)
    { }
};

#endif // DELAY_PROVIDER_HPP
