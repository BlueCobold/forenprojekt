#include "ContactListener.hpp"
#include "model/Entity.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2Fixture.h>

void ContactListener::BeginContact(b2Contact* contact)
{
}
 
void ContactListener::EndContact(b2Contact* contact)
{
    Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if(entityA->getType() == Entity::Ball && entityB->getType() ==  Entity::Teeter)
        entityB->getSoundManager()->play(entityB->getKey());
    if(entityA->getType() == Entity::Teeter && entityB->getType() ==  Entity::Ball)
        entityA->getSoundManager()->play(entityA->getKey());
}
