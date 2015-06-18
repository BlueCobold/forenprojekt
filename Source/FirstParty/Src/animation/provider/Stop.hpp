#pragma once

#ifndef STOP_HPP
#define STOP_HPP

#include "../TimedObject.hpp"
#include "Observer.hpp"
#include "ValueProvider.hpp"

#include <cmath>

/// Stops the owner
class Stop : public ValueProvider, public Observer<Stoppable>
{
public:

    Stop(Stoppable* observed, const CloneCallback cloneCallback = nullptr) : Observer(observed, cloneCallback)
    { }

    virtual float getValue() override
    {
        getObserved()->stop();
        return 0;
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<Stop>(new Stop(getCloneObservable(), getCallback()));
    }
};

#endif //STOP_HPP
