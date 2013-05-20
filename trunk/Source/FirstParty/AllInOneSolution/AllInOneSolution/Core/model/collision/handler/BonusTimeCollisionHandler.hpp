#pragma once

#ifndef BONUS_TIME_COLLISION_HANDLER_HPP
#define BONUS_TIME_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"
#include "../../Level.hpp"

class BonusTimeCollisionHandler : public CollisionHandler
{
public:
    BonusTimeCollisionHandler(Level* level, int bonusTime) 
        : m_level(level), m_bonusTime(bonusTime)
    {
    }

    virtual void onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)
    {
        m_level->addRemainingTime(m_bonusTime);
    }

private:
    Level* m_level;
    int m_bonusTime;
};

#endif // BONUS_TIME_COLLISION_HANDLER_HPP
