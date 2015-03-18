#pragma once

#ifndef CHANGE_BALL_VELOCITY_FILTER_HPP
#define CHANGE_BALL_VELOCITY_FILTER_HPP

#include "ActionFilter.hpp"
#include "../../Ball.hpp"

class ChangeBallVelocityFilter : public ActionFilter
{
public:

    ChangeBallVelocityFilter(
        const float xSpeed,
        const float ySpeed,
        std::unique_ptr<CollisionFilter> child) :
            ActionFilter(std::move(child)),
            m_xSpeed(xSpeed),
            m_ySpeed(ySpeed)
    { }

    virtual ~ChangeBallVelocityFilter()
    { }

    virtual bool shouldCollide(Entity* entityA, Entity* entityB) override
    {
        bool collides = ActionFilter::shouldCollide(entityA, entityB);
        if(collides)
            (dynamic_cast<Ball*>(entityB))->getBody()->SetLinearVelocity(b2Vec2(m_xSpeed, m_ySpeed));
        return collides;
    }

private:

    float m_xSpeed;
    float m_ySpeed;
};

#endif // CHANGE_BALL_VELOCITY_FILTER_HPP
