#pragma once

#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

#include "Stoppable.hpp"

/// Used for objects which have a time-based behavior.
class TimedObject : public Stoppable
{
    float m_startTime;
    float m_currentTime;

public:
    TimedObject() :
        m_startTime(0),
        m_currentTime(0)
    { }

    TimedObject(const float time, bool started = true) :
        Stoppable(started),
        m_startTime(time),
        m_currentTime(time)
    { }

    virtual ~TimedObject()
    { }

    void restartAt(const float time)
    {
        m_startTime = time;
        m_currentTime = time;
        continueNow();
        onRestarted();
    }

    float getPassedTime() const
    {
        return calculatePassedTime();
    }

protected:    
    virtual void onRestarted()
    { }

    void updateCurrentTime(const float time)
    {
        m_currentTime = time;
    }

    float getCurrentTime()
    {
        return m_currentTime;
    }

private:
    virtual float calculatePassedTime() const
    {
        return m_currentTime - m_startTime;
    }
};

#endif //TIMED_OBJECT_HPP

