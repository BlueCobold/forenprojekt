#pragma once

#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "TimedObject.hpp"

class Interpolation : public TimedObject
{
public:

    Interpolation(
        const double startValue,
        const double targetValue,
        const double duration,
        const double time = 0);

    Interpolation();

    void start();
    void update(const double elapsedTime);

    bool isFinished() const { return m_finished; }
    bool isStarted() const { return !isStopped(); }

    double getCurrentValue() const { return m_currentValue; }

    double getTargetValue() const { return m_targetValue; }
    
    void set(const double startValue, const double targetValue, const double duration);

private:

    void onRestarted() override;
    void onStopped() override;

    bool m_finished;

    double m_startValue;
    double m_targetValue;
    double m_currentValue;

    double m_duration;
};

#endif //INTERPOLATION_HPP
