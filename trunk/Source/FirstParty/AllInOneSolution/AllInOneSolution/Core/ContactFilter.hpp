#pragma once

#ifndef CONTACT_FILTER_HPP
#define CONTACT_FILTER_HPP

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactFilter : public b2ContactFilter
{
public:

    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);

};

#endif // CONTACT_FILTER_HPP
