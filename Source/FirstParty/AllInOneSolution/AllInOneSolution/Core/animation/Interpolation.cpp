#include "Interpolation.hpp"

#include <iostream>

Interpolation::Interpolation(
    const float startValue,
	const float targetValue,
	const float duration,
    const float time) :
    TimedObject(time),
	m_startValue(startValue),
	m_targetValue(targetValue),
	m_duration(duration),
    m_currentValue(0),
	m_finished(false),
    m_started(false)
{
    update(0);
}

Interpolation::Interpolation() :
    TimedObject(0),
	m_startValue(0),
	m_targetValue(0),
	m_duration(0),
    m_currentValue(0),
	m_finished(false),
    m_started(false)
{
    update(0);
}

void Interpolation::start()
{
	m_finished = false;
    m_started = true;
    update(getCurrentTime());
}

void Interpolation::stop()
{
    m_started = false;
    m_currentValue = m_targetValue;
}

void Interpolation::restartAt(const float time)
{
    TimedObject::restartAt(time);
    start();
}

void Interpolation::update(const float elapsedTime)
{
    if(!m_started)
        return;

	TimedObject::updateCurrentTime(elapsedTime);

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
