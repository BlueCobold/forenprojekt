#include "Interpolation.hpp"

#include <iostream>

Interpolation::Interpolation(const float startValue,
                             const float targetValue,
                             const float duration,
                             const float time) :
    TimedObject(time, false),
    m_finished(false),
    m_startValue(startValue),
    m_targetValue(targetValue),
    m_currentValue(m_startValue),
    m_duration(duration)
{
    update(0);
}

Interpolation::Interpolation() :
    TimedObject(0, false),
    m_finished(false),
    m_startValue(0),
    m_targetValue(0),
    m_currentValue(0),
    m_duration(0)
{
    updateCurrentTime(0);
}

void Interpolation::start()
{
    m_finished = false;
    continueNow();
    update(getPassedTime());
}

void Interpolation::onStopped()
{
    m_currentValue = m_targetValue;
}

void Interpolation::onRestarted()
{
    start();
}

void Interpolation::update(const float elapsedTime)
{
    if(isStopped() || m_finished)
        return;

    updateCurrentTime(elapsedTime);

    if(getPassedTime() >= m_duration)
    {
        m_currentValue = m_targetValue;
        m_finished = true;
    }
    else
        m_currentValue = m_startValue + (getPassedTime() * (m_targetValue - m_startValue) / m_duration);
}

void Interpolation::set(const float startValue, const float targetValue, const float duration)
{
    m_startValue = startValue;
    m_targetValue = targetValue;
    m_duration = duration;
}
