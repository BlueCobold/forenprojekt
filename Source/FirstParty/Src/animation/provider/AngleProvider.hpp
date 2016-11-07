#pragma once

#ifndef ANGLE_PROVIDER_HPP
#define ANGLE_PROVIDER_HPP

#include "Observer.hpp"
#include "ValueProvider.hpp"
#include "../OrientedObject.hpp"
#include "../../Utility.hpp"

#include <cmath>

/// Returns the angle of the owner
class AngleProvider : public ValueProvider, public Observer<const OrientedObject>
{
    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<AngleProvider>(new AngleProvider(getCloneObservable(), getCallback()));
    }

    double calculateValue() override
    {
        double angle = fmod(utility::toDegree(getObserved().getAngle()), 360.f);
        if(angle < 0)
            return angle + 360;
        return angle;
    }

public:
    AngleProvider(const OrientedObject& observed, const CloneCallback cloneCallback = nullptr) :
        Observer(observed, cloneCallback)
    { }
};

#endif //ANGLE_PROVIDER_HPP
