#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball(float resetTime) :  m_resetTime(resetTime), Entity(Entity::Ball)
{
    m_ballResetTime = 0.0f;
    m_lostBall = false;
}

void Ball::autoResetBall(const float elapsedTime)
{
    // If Ball outside of field
    if(isOutside() && (m_ballResetTime == 0.0f))
        m_ballResetTime = elapsedTime + m_resetTime;
    // if ball back in field
    else if(!isOutside() && (m_ballResetTime > 0.0f))
        m_ballResetTime = 0.0f;
    // if timer up, reset the ball
    else if(m_ballResetTime > 0.0f)
        if(elapsedTime > m_ballResetTime)
        {
            getBody()->SetTransform(getStartPosition(), 0.0f);
            getBody()->SetLinearVelocity(b2Vec2(0,0));
            getBody()->SetAngularVelocity(0.0f);
            m_ballResetTime = 0.0f;
            m_lostBall = true;
        }
}

void Ball::update(const float value)
{
    m_lostBall = false;

    autoResetBall(value);

    Entity::update(value);

    if(m_lostBall)
        setFreeze(1.0f);
}

void Ball::setFieldDimension(b2Vec2 fieldDimension)
{
    m_fieldDimension = fieldDimension;
}

bool Ball::isOutside()
{
    if( (utility::toPixel(getBody()->GetPosition().y) < - 100.f)
        || (utility::toPixel(getBody()->GetPosition().x) < - 100.f)
        || (utility::toPixel(getBody()->GetPosition().x) > (m_fieldDimension.x + 100.f))
        || (utility::toPixel(getBody()->GetPosition().y) > (m_fieldDimension.y + 100.f)))
        return true;
    else
        return false;
}

bool Ball::getBallLost()
{
    return m_lostBall;
}
