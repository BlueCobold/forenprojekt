#pragma once

#ifndef STOP_HPP
#define STOP_HPP

#include "../TimedObject.hpp"

#include <cmath>

/// Stops the owner
class Stop : public ValueProvider
{
private:

    Stoppable* m_owner;

public:
    Stop(Stoppable* owner) : m_owner(owner)
    {
    }

    virtual float getValue() override
    {
        m_owner->stop();
        return 0;
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<Stop>(new Stop(m_owner));
    }
};

#endif //STOP_HPP
