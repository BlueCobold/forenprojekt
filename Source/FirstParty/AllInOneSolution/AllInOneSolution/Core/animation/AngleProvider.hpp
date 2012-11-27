#pragma once

#ifndef ANGLE_PROVIDER_HPP
#define ANGLE_PROVIDER_HPP

#include "OrientedObject.hpp"

/// Returns the angle of the owner
class AngleProvider : public ValueProvider
{
private:

    const OrientedObject* m_owner;

public:
    AngleProvider(const OrientedObject* owner) : m_owner(owner)
    {
    }

    virtual float getValue() const
    {
        return m_owner->getAngle();
    }
};

#endif //ANGLE_PROVIDER_HPP
