#include "ContactFilter.hpp"
#include "../Entity.hpp"

#include <Box2D/Dynamics/b2Fixture.h>

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    Entity* entityA = static_cast<Entity*>(fixtureA->GetBody()->GetUserData());
    Entity* entityB = static_cast<Entity*>(fixtureB->GetBody()->GetUserData());

    if(entityA->getType() == Entity::Target && entityB->getType() == Entity::Ball)
        entityA->kill();
    else if(entityA->getType() == Entity::Ball && entityB->getType() == Entity::Target)
        entityB->kill();

    if(!entityA->isCollideWithBall() && entityB->getType() == Entity::Ball)
        return false;
    else if(entityA->getType() == Entity::Ball && !entityB->isCollideWithBall() )
        return false;

    return true;
}
