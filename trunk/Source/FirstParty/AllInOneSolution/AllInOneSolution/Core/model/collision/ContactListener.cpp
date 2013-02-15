#include "ContactListener.hpp"
#include "../Entity.hpp"

#include "CollisionHandler.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2Fixture.h>

void ContactListener::BeginContact(b2Contact* contact)
{
    Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if(entityA->getType() == Entity::Ball)
    {
        if(entityB->getSoundName().length() > 0)
            entityB->getSoundManager()->play(entityB->getSoundName());
        entityB->onCollide(entityA);
    }
    else if(entityB->getType() == Entity::Ball)
    {
        if(entityA->getSoundName().length() > 0)
            entityA->getSoundManager()->play(entityA->getSoundName());
        entityA->onCollide(entityB);
    }
}
 
void ContactListener::EndContact(b2Contact* contact)
{
}
