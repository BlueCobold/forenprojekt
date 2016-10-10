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

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<Stop>(new Stop(getCloneObservable(), getCallback()));
    }

public:

    Stop(Stoppable& observed, const CloneCallback cloneCallback = nullptr) : Observer(observed, cloneCallback)
    { }

    float getValue() override
    {
        getObserved().stop();
        return 0;
    }
};

#endif //STOP_HPP
