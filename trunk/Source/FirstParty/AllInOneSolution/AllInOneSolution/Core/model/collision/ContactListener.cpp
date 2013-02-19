#include "ContactListener.hpp"
#include "../Entity.hpp"

#include "./handler/CollisionHandler.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2Fixture.h>

void ContactListener::BeginContact(b2Contact* contact)
{
    Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());


    float 
    velocityA = 
    abs(contact->GetFixtureA()->GetBody()->GetLinearVelocity().x) + abs(contact->GetFixtureA()->GetBody()->GetLinearVelocity().y),
    velocityB =
    abs(contact->GetFixtureB()->GetBody()->GetLinearVelocity().x) + abs(contact->GetFixtureB()->GetBody()->GetLinearVelocity().y);

    if(entityA->getType() == Entity::Ball)
    {
        if(entityB->getSoundName().length() > 0)
            entityB->getSoundManager()->play(entityB->getSoundName(), velocityA);
        entityB->onCollide(entityA);
    }
    else if(entityB->getType() == Entity::Ball)
    {
        if(entityA->getSoundName().length() > 0)
            entityA->getSoundManager()->play(entityA->getSoundName(), velocityB);
        entityA->onCollide(entityB);
    }
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

    if(entityA->getType() != Entity::Ball && entityB->getType() == Entity::Ball)
    {
        if(!entityA->shouldCollide(entityB))
            return false;
        if(entityA->getType() == Entity::Target)
            entityA->kill();
        return entityA->doesCollideWithBall();
    }
    else if(entityB->getType() != Entity::Ball && entityA->getType() == Entity::Ball)
    {
        if(!entityB->shouldCollide(entityA))
            return false;
        if(entityB->getType() == Entity::Target)
            entityB->kill();
        return entityB->doesCollideWithBall();
    }

    return true;
}
