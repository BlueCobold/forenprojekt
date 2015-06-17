#pragma once

#ifndef TIME_PROVIDER_HPP
#define TIME_PROVIDER_HPP

#include "ValueProvider.hpp"
#include "../TimedObject.hpp"

/// Returns the time of the owner.
class TimeProvider : public ValueProvider
{
private:

    const TimedObject* m_owner;

public:
    TimeProvider(const TimedObject* owner) : m_owner(owner)
    {
    }

    virtual float getValue() override
    {
        return m_owner->getPassedTime();
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<TimeProvider>(new TimeProvider(m_owner));
    }
};

#endif //TIME_PROVIDER_HPP
