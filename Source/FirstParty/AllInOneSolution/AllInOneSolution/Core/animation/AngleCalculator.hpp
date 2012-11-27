#pragma once

#ifndef ANGLE_CALCULATOR_HPP
#define ANGLE_CALCULATOR_HPP

#include "ValueCalculator.hpp"
#include "AngleProvider.hpp"
#include "../Utility.hpp"

#include <cmath>
#include <algorithm>
#include <memory> // unique_ptr

const float M_2_PI = 2*3.14159265358979323846f;

/// Returns a specific calculated value calculated from the provider's value.
class AngleCalculator : public ValueCalculator
{
private:


    float m_min;
    float m_step;
    int m_numFrames;

public:

    AngleCalculator(std::unique_ptr<AngleProvider> provider,
        const float minAngle, const float step, const int numFrames) :
        ValueCalculator(std::move(provider)),
        m_min(utility::toRadian<float, float>(minAngle)),
        m_step(utility::toRadian<float, float>(step)),
        m_numFrames(numFrames)
    {
    }

    virtual float calculateValue() const
    {
        float angle = fmod(getProvider()->getValue(), M_2_PI);
        float range = m_step * m_numFrames;
        float maxAngle = m_min + range;
        if((angle < 0) && (fabsf(M_2_PI - range) < 0.01))
            angle += M_2_PI;
        float steps = std::max(m_min, std::min(maxAngle, angle));
        return steps / m_step;
    }
};

#endif //ANGLE_CALCULATOR_HPP
