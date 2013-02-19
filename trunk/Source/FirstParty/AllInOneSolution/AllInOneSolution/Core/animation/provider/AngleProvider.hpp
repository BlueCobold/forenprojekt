#pragma once

#ifndef ANGLE_PROVIDER_HPP
#define ANGLE_PROVIDER_HPP

#include "../OrientedObject.hpp"
#include "../Utility.hpp"

#include <cmath>

/// Returns the angle of the owner
class AngleProvider : public ValueProvider
{
private:

    const OrientedObject* m_owner;

public:
    AngleProvider(const OrientedObject* owner) : m_owner(owner)
    {
    }

    virtual float getValue()
    {
        float angle = fmod(utility::toDegree<float, float>(m_owner->getAngle()), 360.f);
        if(angle < 0)
            return angle + 360;
        return angle;
    }
};

#endif //ANGLE_PROVIDER_HPP
