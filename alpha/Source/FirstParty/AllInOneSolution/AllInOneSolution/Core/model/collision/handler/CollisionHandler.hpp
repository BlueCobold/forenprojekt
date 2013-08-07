#pragma once

#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

#include <Box2D/Common/b2Math.h>

class Entity;

class CollisionHandler
{
public:
    virtual ~CollisionHandler()
    { }

    virtual void onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse) = 0;
};

#endif // COLLISION_HANDLER_HPP
