#include "ContactListener.hpp"
#include "../Entity.hpp"
#include "../../model/Ball.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <exception>

ContactListener::ContactListener(CollisionHandler* handler, CollisionFilter* filter)
    : m_handler(handler),
    m_filter(filter)
{
    if(handler == nullptr || filter == nullptr)
        throw std::exception("Parameters may not be null");
}

void ContactListener::BeginContact(b2Contact* contact)
{
    Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    m_handler->onCollision(entityA, entityB);
}
 
void ContactListener::EndContact(b2Contact* contact)
{
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    if(!shouldCollide(contact->GetFixtureA(), contact->GetFixtureB()))
        contact->SetEnabled(false);
}

bool ContactListener::shouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    Entity* entityA = static_cast<Entity*>(fixtureA->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(fixtureB->GetBody()->GetUserData());

    return m_filter->shouldCollide(entityA, entityB);
}
