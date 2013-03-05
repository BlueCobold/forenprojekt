#pragma once

#ifndef NEVER_HPP
#define NEVER_HPP

class Entity;

class Never : public CollisionFilter
{
public:

    virtual bool shouldCollide(Entity* entityA, Entity* entityB)
    {
        return false;
    }
};

#endif // COLLISION_FILTER_HPP
