#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball() :  Entity(Entity::Ball)
{
    m_ballResetTime = 0.0f;
    m_startPosition.x = 0.0f;
    m_startPosition.y = 0.0f;
}
void Ball::autoResetBall(const float elapsedTime)
{
    // If Ball outside of field
    if(((utility::toPixel(m_body->GetPosition().x) < - 100.f) ||
        (utility::toPixel(m_body->GetPosition().x) > (m_fieldDimension.x + 100.f)) ||
        (utility::toPixel(m_body->GetPosition().y) > (m_fieldDimension.y + 100.f))) &&
       (m_ballResetTime == 0.0f))
        m_ballResetTime = elapsedTime + 1.0f;
    // if ball back in field
    else if((utility::toPixel(m_body->GetPosition().x) > - 100.f) &&
            (utility::toPixel(m_body->GetPosition().x) < (m_fieldDimension.x + 100.f)) &&
            (utility::toPixel(m_body->GetPosition().y) < (m_fieldDimension.y + 100.f)) &&
            (m_ballResetTime > 0.0f))
        m_ballResetTime = 0.0f;
    // if timer up, reset the ball
    else if(m_ballResetTime > 0.0f)
        if(elapsedTime > m_ballResetTime)
        {
            m_body->SetTransform(m_startPosition,0.0f);
            m_ballResetTime = 0.0f;
        }
}

void Ball::update(const float value)
{
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