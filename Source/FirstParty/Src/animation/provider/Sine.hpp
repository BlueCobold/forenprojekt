#pragma once

#ifndef SINE_HPP
#define SINE_HPP

#include "SingleProvider.hpp"
#include "ValueProvider.hpp"
#include "../../Utility.hpp"

#include <memory>
#include <cmath>

/// Returns the sine of the value of the passed provider
class Sine : public SingleProvider
{
private:
    float m_amplitude;
    float m_period;

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Sine>(new Sine(getProvider()->clone(), m_amplitude, m_period));
    }

    double calculateValue() override
    {
        return m_amplitude * sin(utility::toRadian(m_period * getProvider()->getValue()));
    }

public:
    Sine(std::unique_ptr<ValueProvider> provider, float amplitude = 1.f, float period = 1.f) :
        SingleProvider(std::move(provider)),
        m_amplitude(amplitude),
        m_period(period)
    { }
};

#endif //SINE_HPP
