#pragma once

#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

#include "Stoppable.hpp"

/// Used for objects which have a time-based behavior.
class TimedObject : public Stoppable
{
public:
    TimedObject() : m_startTime(0), m_currentTime(0)
    { }

    TimedObject(const float time)
    {
        restartAt(time);
    }

    virtual ~TimedObject()
    { }

    virtual void restartAt(const float time)
    {
        m_startTime = time;
        m_currentTime = time;
        Stoppable::continueNow();
    }

    virtual float getPassedTime() const
    {
        return m_currentTime - m_startTime;
    }

protected:

    void updateCurrentTime(const float time)
    {
        m_currentTime = time;
    }

    float getCurrentTime()
    {
        return m_currentTime;
    }

private:

    float m_startTime;
    float m_currentTime;
};

#endif //TIMED_OBJECT_HPP

