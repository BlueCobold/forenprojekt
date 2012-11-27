#pragma once

#ifndef TIME_CALCULATOR_HPP
#define TIME_CALCULATOR_HPP

#include "ValueCalculator.hpp"
#include "TimeProvider.hpp"

#include <cmath>
#include <memory> // unique_ptr

/// Returns a specific calculated value calculated from the provider's time.
class TimeCalculator : public ValueCalculator
{
private:

    float m_step;
    int m_numFrames;

public:

    TimeCalculator(std::unique_ptr<TimeProvider> provider, const float step, const int numFrames)
        : ValueCalculator(std::move(provider)), m_step(step), m_numFrames(numFrames)
    {
    }

    virtual float calculateValue() const
    {
        float steps = getProvider()->getValue() / m_step;
        steps = fmod(steps, m_numFrames);
        return steps;
    }
};

#endif //TIME_CALCULATOR_HPP
