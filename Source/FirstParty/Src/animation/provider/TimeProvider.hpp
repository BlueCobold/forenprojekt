#pragma once

#ifndef TIME_PROVIDER_HPP
#define TIME_PROVIDER_HPP

#include "Observer.hpp"
#include "ValueProvider.hpp"
#include "../TimedObject.hpp"

/// Returns the time of the owner.
class TimeProvider : public ValueProvider, public Observer<const TimedObject>
{
public:

    TimeProvider(const TimedObject& observer, const CloneCallback cloneCallback = nullptr) :
        Observer(observer, cloneCallback)
    { }

    virtual float getValue() override
    {
        return getObserved().getPassedTime();
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<TimeProvider>(new TimeProvider(getCloneObservable(), getCallback()));
    }
};

#endif //TIME_PROVIDER_HPP
