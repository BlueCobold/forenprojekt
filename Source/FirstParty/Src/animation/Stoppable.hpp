#pragma once

#ifndef STOPPABLE_HPP
#define STOPPABLE_HPP

/// Used for objects which have a stoppable behavior.
class Stoppable
{
public:
    Stoppable() : m_stopped(false)
    { }

    Stoppable(bool started) : m_stopped(!started)
    { }

    virtual ~Stoppable()
    { }

    void stop()
    {
        m_stopped = true;
        onStopped();
    }

    bool isStopped() const
    {
        return m_stopped;
    }

protected:

    virtual void onStopped()
    { }

    void continueNow()
    {
        m_stopped = false;
    }

private:

    bool m_stopped;
};

#endif //STOPPABLE_HPP
