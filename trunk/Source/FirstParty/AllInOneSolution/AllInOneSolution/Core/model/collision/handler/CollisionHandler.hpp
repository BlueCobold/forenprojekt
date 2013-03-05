#pragma once

#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

class Entity;

class CollisionHandler
{
public:
    virtual ~CollisionHandler()
    { }

    virtual void onCollision(Entity* entityA, Entity* entityB) = 0;
};

#endif // COLLISION_HANDLER_HPP
