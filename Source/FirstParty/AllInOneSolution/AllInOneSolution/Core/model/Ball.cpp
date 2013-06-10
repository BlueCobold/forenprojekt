#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball(float resetTime, const Entity* spawnAnimationEntity) :
    m_resetTime(resetTime),
    m_hideTime(1.0f),
    m_blowUpTime(1.0f),
    Entity(Entity::Ball),
    m_spawnAnimationEntity(spawnAnimationEntity),
    m_lostBall(false),
    m_ballResetTime(0.0f),
    m_blownUp(false)
{ }

void Ball::autoResetBall(const float elapsedTime)
{
    // If Ball outside of field
    if(isOutside() && (m_ballResetTime == 0.0f))
        m_ballResetTime = elapsedTime + m_resetTime;
    else if(blownUp() && m_ballResetTime == 0.0f)
        m_ballResetTime = elapsedTime + m_blowUpTime;
    // if ball back in field
    else if(!blownUp() && !isOutside() && (m_ballResetTime > 0.0f) && elapsedTime < m_ballResetTime)
        m_ballResetTime = 0.0f;
    // if timer up, reset the ball
    else if(m_ballResetTime > 0.0f)
        if(elapsedTime > m_ballResetTime + m_hideTime && frozen())
        {
            unfreeze();
            unhide();
            m_ballResetTime = 0.0f;
        }
        else if(elapsedTime > m_ballResetTime && (!frozen() || blownUp()))
        {
            m_blownUp = false;
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

    if(blownUp() && !frozen())
        freeze();
    if(blownUp() && !hidden())
        hide();

    autoResetBall(value);

    Entity::update(value);
}

const Entity* Ball::getSpawnAnimationEntity() const
{
    return m_spawnAnimationEntity;
}

void Ball::blowUp()
{
    m_blownUp = true;
}

bool Ball::blownUp()
{
    return m_blownUp;
}

void Ball::checkpointReached(const b2Vec2 position)
{
    if(m_checkpointHandler != nullptr &&
        (fabsf(position.x - m_spawnPosition.x)>0.01f || fabsf(position.y - m_spawnPosition.y)>0.01f) )
        m_checkpointHandler();
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

void Ball::registerForCheckpointChanges(std::function<void()> handler)
{
    m_checkpointHandler = handler;
}