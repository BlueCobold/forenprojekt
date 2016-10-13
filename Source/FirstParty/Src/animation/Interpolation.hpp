#pragma once

#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "TimedObject.hpp"

class Interpolation : public TimedObject
{
public:

    Interpolation(
        const float startValue,
        const float targetValue,
        const float duration,
        const float time = 0);

    Interpolation();

    void start();
    void update(const float elapsedTime);

    bool isFinished() const { return m_finished; }
    bool isStarted() const { return !isStopped(); }

    float getCurrentValue() const { return m_currentValue; }

    float getTargetValue() const { return m_targetValue; }
    
    void set(const float startValue, const float targetValue, const float duration);

private:

    void onRestarted() override;
    void onStopped() override;

    bool m_finished;

    float m_startValue;
    float m_targetValue;
    float m_currentValue;

    float m_duration;
};

#endif //INTERPOLATION_HPP
