#pragma once

#ifndef COLLISION_FILTER_HPP
#define COLLISION_FILTER_HPP

class Entity;

class CollisionFilter
{
protected:
    virtual bool onShouldCollide(Entity& entityA, Entity& entityB) = 0;

public:
    virtual ~CollisionFilter()
    { }

    bool shouldCollide(Entity& entityA, Entity& entityB)
    {
        return onShouldCollide(entityA, entityB);
    }
};

#endif // COLLISION_FILTER_HPP
