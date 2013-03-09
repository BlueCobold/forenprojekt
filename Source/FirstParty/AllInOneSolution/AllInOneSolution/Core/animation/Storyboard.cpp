#include "Storyboard.hpp"
#include <iostream>
Storyboard::Storyboard(const float startValue,
					   const float targetValue,
					   const float duration):
	m_startValue(startValue),
	m_targetValue(targetValue),
	m_duration(duration),
	m_finished(false)
{
    TimedObject::restartAt(0);
    m_lastTime = 0.f;
    m_finishTime = 0.f;
}

void Storyboard::start()
{
    m_finishTime = m_duration + getPassedTime();
	m_finished = false;
}

void Storyboard::update(const float elapsedTime)
{
	TimedObject::updateCurrentTime(elapsedTime);

	if (getPassedTime() >= m_finishTime)
	{
		m_currentValue = m_targetValue;
		m_finished = true;
	}
	else
		m_currentValue = m_currentValue + ((getPassedTime() - m_lastTime) * (m_targetValue - m_startValue) / m_duration);

    m_lastTime = getPassedTime();
}

void Storyboard::set(const float startValue, const float targetValue, const float duration)
{
    m_startValue = startValue;
    m_targetValue = targetValue;
    m_duration = duration;
}