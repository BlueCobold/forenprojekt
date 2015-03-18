#pragma once

#ifndef STOPPABLE_HPP
#define STOPPABLE_HPP

/// Used for objects which have a stoppable behavior.
class Stoppable
{
public:
    Stoppable() : m_stopped(false)
    { }

    virtual ~Stoppable()
    { }

    virtual void stop()
    {
        m_stopped = true;
    }

    virtual bool isStopped() const
    {
        return m_stopped;
    }

protected:

    virtual void continueNow()
    {
        m_stopped = false;
    }

private:

    bool m_stopped;
};

#endif //STOPPABLE_HPP
