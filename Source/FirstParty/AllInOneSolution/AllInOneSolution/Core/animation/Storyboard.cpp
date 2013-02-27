
#include "Storyboard.hpp"

Storyboard::Storyboard(const float startValue,
					   const float targetValue,
					   const float duration):
	m_startValue(startValue),
	m_targetValue(targetValue),
	m_duration(duration),
	m_finished(false)
{
}

void Storyboard::start()
{
	TimedObject::restartAt(0);
	m_finished = false;
}

void Storyboard::update(const float elapsedTime)
{
	TimedObject::updateCurrentTime(elapsedTime);

	if (getPassedTime() >= m_duration)
	{
		m_currentValue = m_targetValue;
		m_finished = true;
	}
	else
	{
		m_currentValue = m_startValue + getPassedTime() * (m_targetValue - m_startValue);
	}
}