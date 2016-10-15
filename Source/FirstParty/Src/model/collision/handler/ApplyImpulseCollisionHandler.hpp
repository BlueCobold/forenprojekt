#pragma once

#ifndef APPLY_IMPULSE_COLLISION_HANDLER_HPP
#define APPLY_IMPULSE_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"
#include "../../Entity.hpp"

class ApplyImpulseCollisionHandler : public CollisionHandler
{
public:
    ApplyImpulseCollisionHandler(float xImpulse, float yImpulse) 
        : m_xImpulse(xImpulse), m_yImpulse(yImpulse)
    { }

private:
    float m_xImpulse;
    float m_yImpulse;

    void handleCollision(Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse) override
    {
        b2Body* ballBody;

        if(entityA.getType() == Entity::Ball)
            ballBody = entityA.getBody();
        else if(entityB.getType() == Entity::Ball)
            ballBody = entityB.getBody();
        else
            throw std::runtime_error(utility::replace(utility::translateKey("EntityNoCollision"), "Ball"));

        ballBody->ApplyLinearImpulse(b2Vec2(m_xImpulse, m_yImpulse), ballBody->GetWorldCenter(), false);
    }
};

#endif // APPLY_IMPULSE_COLLISION_HANDLER_HPP
