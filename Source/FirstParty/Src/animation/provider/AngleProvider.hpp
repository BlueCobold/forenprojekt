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

public:
    AngleProvider(const OrientedObject* observed, const CloneCallback cloneCallback = nullptr) :
        Observer(observed, cloneCallback)
    { }

    virtual float getValue() override
    {
        float angle = fmod(utility::toDegree<float, float>(getObserved()->getAngle()), 360.f);
        if(angle < 0)
            return angle + 360;
        return angle;
    }

    virtual std::unique_ptr<ValueProvider> clone() const override
    {
        return std::unique_ptr<AngleProvider>(new AngleProvider(getCloneObservable(), getCallback()));
    }
};

#endif //ANGLE_PROVIDER_HPP
