#pragma once

#ifndef ALWAYS_HPP
#define ALWAYS_HPP

class Entity;

class Always : public CollisionFilter
{
public:

    virtual bool shouldCollide(Entity* entityA, Entity* entityB)
    {
        return true;
    }
};

#endif // COLLISION_FILTER_HPP
