#pragma once

#ifndef STORYBOARD_HPP
#define STORYBOARD_HPP

#include "TimedObject.hpp"

class Storyboard : public TimedObject
{
public:

	Storyboard(const float startValue,
			   const float targetValue,
			   const float duration);

	void start();
	void update(const float elapsedTime);

	bool isFinished() const { return m_finished; }

	float getCurrentValue() const { return m_currentValue; }

    float getTargetValue() const { return m_targetValue; }
    
    void set(const float startValue, const float targetValue, const float duration);

private:

	bool m_finished;

	float m_startValue;
	float m_targetValue;
	float m_currentValue;

	float m_duration;

    float m_lastTime;
    float m_finishTime;
};

#endif //TIMED_OBJECT_HPP

