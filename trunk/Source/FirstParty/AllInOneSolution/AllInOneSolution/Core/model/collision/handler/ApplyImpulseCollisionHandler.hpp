#pragma once

#ifndef APPLY_IMPULSE_COLLISION_HANDLER_HPP
#define APPLY_IMPULSE_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"

class ApplyImpulseCollisionHandler : public CollisionHandler
{
public:
    ApplyImpulseCollisionHandler(float xImpulse, float yImpulse) 
        : m_xImpulse(xImpulse), m_yImpulse(yImpulse)
    {
    }

    virtual void onCollision(Entity* entityA, Entity* entityB)
    {
        b2Body* ballBody;

        if(entityA->getType() == Entity::Ball)
            ballBody = entityA->getBody();
        else if(entityB->getType() == Entity::Ball)
            ballBody = entityB->getBody();
        else
            throw std::runtime_error("ApplyImpulseCollisionHandler: Ball does not collide.");

        ballBody->ApplyLinearImpulse(b2Vec2(m_xImpulse, m_yImpulse), ballBody->GetWorldCenter(), false);
    }

private:
    float m_xImpulse;
    float m_yImpulse;
};

#endif // APPLY_IMPULSE_COLLISION_HANDLER_HPP
