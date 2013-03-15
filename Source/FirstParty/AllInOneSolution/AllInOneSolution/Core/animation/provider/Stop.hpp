#pragma once

#ifndef STOP_HPP
#define STOP_HPP

#include "../TimedObject.hpp"
#include "../Utility.hpp"

#include <cmath>

/// Stops the owner
class Stop : public ValueProvider
{
private:

    TimedObject* m_owner;

public:
    Stop(TimedObject* owner) : m_owner(owner)
    {
    }

    virtual float getValue()
    {
        m_owner->stop();
        return 0;
    }
};

#endif //STOP_HPP
