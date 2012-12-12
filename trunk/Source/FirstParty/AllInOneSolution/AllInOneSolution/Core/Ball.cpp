#include "Ball.hpp"

Ball::Ball(b2Vec2 startPosition) : m_startPosition(startPosition), Entity(Entity::Ball)
{
}

void Ball::autoResetBall(const float elapsedTime, float width, float height)
{
    if((m_body->GetPosition().x > (width + 100.0f) ||
        m_body->GetPosition().x < - 100.0f ||
        m_body->GetPosition().y > (height + 100.0f)) &&
        m_ballResetTime == 0.0f)
    {
        m_ballResetTime = elapsedTime + 1.0f;
    }
    else if(elapsedTime > m_ballResetTime)
    {
        //m_body->SetPosition(m_startPosition);
        //m_ball->getPosition().x = 100.0f;
        //m_ball->getPosition().y = 100.0f;
    }
}

void Ball::update(const float elapsedTime)
{
}