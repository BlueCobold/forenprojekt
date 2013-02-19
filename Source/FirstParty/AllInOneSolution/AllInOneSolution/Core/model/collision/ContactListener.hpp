#pragma once

#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactListener : public b2ContactListener
{
public:

    void BeginContact(b2Contact* contact);

    void EndContact(b2Contact* contact);

    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

private:

    bool shouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};

#endif // CONTACT_LISTENER_HPP

