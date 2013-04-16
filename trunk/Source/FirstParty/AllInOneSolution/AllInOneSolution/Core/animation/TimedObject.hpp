#pragma once

#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

/// Used for objects which have a time-based behavior.
class TimedObject
{
public:
    TimedObject() : m_stopped(false), m_startTime(0), m_currentTime(0)
    { }

    TimedObject(const float time) : m_stopped(false)
    {
        restartAt(time);
    }

    virtual ~TimedObject()
    { }

    virtual void restartAt(const float time)
    {
        m_startTime = time;
        m_currentTime = time;
        m_stopped = false;
    }

    virtual float getPassedTime() const
    {
        return m_currentTime - m_startTime;
    }

    virtual void stop()
    {
        m_stopped = true;
    }

    virtual bool isStopped() const
    {
        return m_stopped;
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
    bool m_stopped;
};

#endif //TIMED_OBJECT_HPP

