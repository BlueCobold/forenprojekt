#pragma once

#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

/// Used for objects which have a time-based behavior.
class TimedObject
{
public:
    TimedObject() : m_stopped(false)
    { }

    virtual ~TimedObject()
    { }

    virtual void restartAt(const float time)
    {
        m_startTime = time;
        m_currentTime = time;
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

private:

    float m_startTime;
    float m_currentTime;
    bool m_stopped;
};

#endif //TIMED_OBJECT_HPP

