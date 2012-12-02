#pragma once

#ifndef TIMED_OBJECT_HPP
#define TIMED_OBJECT_HPP

/// Used for objects which have a time-based behavior.
class TimedObject
{
public:

    virtual void restartAt(const float time)
    {
        m_startTime = time;
        m_currentTime = time;
    }

    float getPassedTime() const
    {
        return m_currentTime - m_startTime;
    }

protected:

    void updateCurrentTime(const float time)
    {
        m_currentTime = time;
    }

private:

    float m_startTime;
    float m_currentTime;
};

#endif //TIMED_OBJECT_HPP

