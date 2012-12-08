#include "ContactListener.hpp"
#include "Entity.hpp"

#include <Box2D/Dynamics/Contacts/b2Contact.h>

void ContactListener::BeginContact(b2Contact* contact)
{
    Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if(entityA->getType() == Entity::Target && entityB->getType() == Entity::Ball)
        entityA->kill();
    else if(entityA->getType() == Entity::Ball && entityB->getType() == Entity::Target)
        entityB->kill();

    
}
 
void ContactListener::EndContact(b2Contact* contact)
{
}
