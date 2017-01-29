#pragma once

#ifndef BALL_ANGLE_FILTER_HPP
#define BALL_ANGLE_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

#include <algorithm>

#include <iostream>
class BallAngleFilter : public CollisionFilter
{
public:
    BallAngleFilter(float minAngle, float maxAngle) :
        m_overflow(minAngle > maxAngle),
        m_minAngle(minAngle),
        m_maxAngle(maxAngle)
    { }

    virtual ~BallAngleFilter()
    { }

private:
    bool m_overflow;
    float m_minAngle;
    float m_maxAngle;

    bool onShouldCollide(Entity& entityA, Entity& entityB) override
    {
        auto direction = dynamic_cast<Ball&>(entityB).getBody()->GetLinearVelocity();
        auto angle = atan2f(direction.x, direction.y);
        if(m_overflow)
            return !(angle > m_maxAngle && angle < m_minAngle);
        else
            return angle >= m_minAngle && angle <= m_maxAngle;
    }
};

#endif // CHANGE_BALL_VELOCITY_FILTER_HPP
