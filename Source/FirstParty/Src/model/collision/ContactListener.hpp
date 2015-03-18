#pragma once

#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include "./handler/CollisionHandler.hpp"
#include "./filter/CollisionFilter.hpp"

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactListener : public b2ContactListener
{
public:

    ContactListener();
    ContactListener(CollisionHandler* handler, CollisionFilter* filter);

    void BeginContact(b2Contact* contact);

    void EndContact(b2Contact* contact);

    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:

    bool shouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
    CollisionHandler* m_handler;
    CollisionFilter* m_filter;
};

#endif // CONTACT_LISTENER_HPP

