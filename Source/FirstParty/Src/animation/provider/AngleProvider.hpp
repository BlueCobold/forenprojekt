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
private:

    std::unique_ptr<ValueProvider> doClone() const override
    {
        return std::unique_ptr<AngleProvider>(new AngleProvider(getCloneObservable(), getCallback()));
    }

public:
    AngleProvider(const OrientedObject& observed, const CloneCallback cloneCallback = nullptr) :
        Observer(observed, cloneCallback)
    { }

    float getValue() override
    {
        float angle = fmod(utility::toDegree(getObserved().getAngle()), 360.f);
        if(angle < 0)
            return angle + 360;
        return angle;
    }
};

#endif //ANGLE_PROVIDER_HPP
