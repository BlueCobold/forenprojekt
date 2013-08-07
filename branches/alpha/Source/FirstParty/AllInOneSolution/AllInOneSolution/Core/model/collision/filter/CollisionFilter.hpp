#pragma once

#ifndef COLLISION_FILTER_HPP
#define COLLISION_FILTER_HPP

class Entity;

class CollisionFilter
{
public:
    virtual ~CollisionFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) = 0;
};

#endif // COLLISION_FILTER_HPP
