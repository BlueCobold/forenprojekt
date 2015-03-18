#pragma once

#ifndef BALL_TELEPORT_COLLISION_HANDLER_HPP
#define BALL_TELEPORT_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"
#include "../../../Utility.hpp"

class BallTeleportCollisionHandler : public CollisionHandler
{
public:
    BallTeleportCollisionHandler(float x, float y, bool killSpeed) 
        : m_x(utility::toMeter(x)), m_y(utility::toMeter(y)), m_kill(killSpeed)
    {
    }

    virtual void onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse) override
    {
        if(entityA && entityA->getType() == Entity::Ball)
            dynamic_cast<Ball*>(entityA)->teleportTo(m_x, m_y, m_kill);
        else if(entityB && entityB->getType() == Entity::Ball)
            dynamic_cast<Ball*>(entityB)->teleportTo(m_x, m_y, m_kill);
        else
            throw std::runtime_error(utility::replace(utility::translateKey("EntityNoCollision"), "Ball"));
    }

private:
    float m_x;
    float m_y;
    bool m_kill;
};

#endif // BALL_TELEPORT_COLLISION_HANDLER_HPP
