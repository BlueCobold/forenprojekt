#pragma once

#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

class Entity;

class CollisionHandler
{
public:

    virtual void OnCollision(Entity* entityA, Entity* entityB) = 0;
};

#endif // COLLISION_HANDLER_HPP
