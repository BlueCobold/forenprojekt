#pragma once

#ifndef GENERIC_COLLISION_HANDLER_HPP
#define GENERIC_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"

#include <functional>

class GenericCollisionHandler : public CollisionHandler
{
    typedef std::function<void(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)> Handler;

public:

    GenericCollisionHandler(const Handler& handler) :
        m_handler(handler)
    {
    }

    virtual void onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse) override
    {
        if(m_handler != nullptr)
            m_handler(entityA, entityB, point, impulse);
    }

private:

    Handler m_handler;
};

#endif // GENERIC_COLLISION_HANDLER_HPP
