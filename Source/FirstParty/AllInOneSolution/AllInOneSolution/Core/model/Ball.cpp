#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball(float resetTime, const Entity* spawnAnimationEntity) :
    m_resetTime(resetTime),
    m_hideTime(1.0f),
    Entity(Entity::Ball),
    m_spawnAnimationEntity(spawnAnimationEntity),
    m_lostBall(false),
    m_ballResetTime(0.0f)
{ }

void Ball::autoResetBall(const float elapsedTime)
{
    // If Ball outside of field
    if(isOutside() && (m_ballResetTime == 0.0f))
        m_ballResetTime = elapsedTime + m_resetTime;
    // if ball back in field
    else if(!isOutside() && (m_ballResetTime > 0.0f) && elapsedTime < m_ballResetTime)
        m_ballResetTime = 0.0f;
    // if timer up, reset the ball
    else if(m_ballResetTime > 0.0f)
        if(elapsedTime > m_ballResetTime + m_hideTime && frozen())
        {
            unfreeze();
            unhide();
            m_ballResetTime = 0.0f;
        }
        else if(elapsedTime > m_ballResetTime && !frozen())
        {
            getBody()->SetTransform(m_spawnPosition, 0.0f);
            getBody()->SetLinearVelocity(b2Vec2(0,0));
            getBody()->SetAngularVelocity(0.0f);
            m_lostBall = true;
            freeze();
            hide();
        }
}

void Ball::restartAt(const float value)
{
    Entity::restartAt(value);
    m_spawnPosition = getStartPosition();
}

void Ball::update(const float value)
{
    m_lostBall = false;

    autoResetBall(value);

    Entity::update(value);
}

const Entity* Ball::getSpawnAnimationEntity() const
{
    return m_spawnAnimationEntity;
}

void Ball::checkpointReached(const b2Vec2 position)
{
    m_spawnPosition = position;
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
