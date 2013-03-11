#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball(float resetTime) :  m_resetTime(resetTime), Entity(Entity::Ball)
{
    m_ballResetTime = 0.0f;
    m_startPosition.x = 0.0f;
    m_startPosition.y = 0.0f;
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
            m_body->SetTransform(m_startPosition,0.0f);
            m_body->SetLinearVelocity(b2Vec2(0,0));
            m_body->SetAngularVelocity(0.0f);
            m_ballResetTime = 0.0f;
            m_lostBall = true;
        }
}

void Ball::update(const float value)
{
    m_lostBall = false;

    autoResetBall(value);

    Entity::update(value);
}

void Ball::setStartPosition(b2Vec2 startPosition)
{
    m_startPosition = startPosition;
}

void Ball::setFieldDimension(b2Vec2 fieldDimension)
{
    m_fieldDimension = fieldDimension;
}

bool Ball::isOutside()
{
    if( (utility::toPixel(m_body->GetPosition().y) < - 100.f)
        || (utility::toPixel(m_body->GetPosition().x) < - 100.f)
        || (utility::toPixel(m_body->GetPosition().x) > (m_fieldDimension.x + 100.f))
        || (utility::toPixel(m_body->GetPosition().y) > (m_fieldDimension.y + 100.f)))
        return true;
    else
        return false;
}

bool Ball::getBallLost()
{
    return m_lostBall;
}
