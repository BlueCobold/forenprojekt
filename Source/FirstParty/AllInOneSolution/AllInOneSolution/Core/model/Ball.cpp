#include "Ball.hpp"
#include "../Utility.hpp"
#include <iostream>

Ball::Ball(float resetTime, const Entity* spawnAnimationEntity, const Entity* killAnimationEntity) :
    m_resetTime(resetTime),
    m_hideTime(1.0f),
    m_blowUpTime(1.0f),
    Entity(Entity::Ball),
    m_spawnAnimationEntity(spawnAnimationEntity),
    m_killAnimationEntity(killAnimationEntity),
    m_lostBall(false),
    m_ballResetTime(0.0f),
    m_blownUp(false),
    m_isTeleporting(false)
{ }

void Ball::autoResetBall(const float elapsedTime)
{
    if(m_isTeleporting)
    {
        if(m_resetSpeedOnTeleport)
        {
            getBody()->SetLinearVelocity(b2Vec2(0, 0));
            getBody()->SetAngularVelocity(0.0f);
        } 
        getBody()->SetTransform(m_teleportPosition, 0.0f);
        m_isTeleporting = false;
    }
    // If Ball outside of field
    else if(isOutside() && (m_ballResetTime == 0.0f))
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
            if(m_trail != nullptr)
                m_trail->setTo(getPosition().x, getPosition().y);
        }
        else if(elapsedTime > m_ballResetTime && (!frozen() || blownUp()))
        {
            m_blownUp = false;
            getBody()->SetTransform(m_spawnPosition, 0.0f);
            getBody()->SetLinearVelocity(b2Vec2(0, 0));
            getBody()->SetAngularVelocity(0.0f);
            m_lostBall = true;
            freeze();
            hide();
        }
}

void Ball::teleportTo(const float x, const float y, const bool resetSpeed)
{
    m_teleportPosition = b2Vec2(x, y);
    m_isTeleporting = true;
    m_resetSpeedOnTeleport = resetSpeed;
}

void Ball::restartAt(const float value)
{
    Entity::restartAt(value);
    m_spawnPosition = getStartPosition();
    if(m_trail != nullptr)
        m_trail->setTo(getPosition().x, getPosition().y);
}

void Ball::update(const float value)
{
    m_lostBall = false;

    if(blownUp() && !frozen())
        freeze();
    if(blownUp() && !hidden())
        hide();

    if(m_trail != nullptr && !frozen() && !hidden())
    {
        if(value <= 0 || !isSpeeding())
            m_trail->setTo(getPosition().x, getPosition().y);
        else
            m_trail->moveTo(getPosition().x, getPosition().y);
    }

    autoResetBall(value);

    Entity::update(value);
    if(m_trail != nullptr)
    {
        m_trail->update();
    }
}

float Ball::getValueOf(const std::string& name) const
{
    if(m_trail == nullptr)
        return Entity::getValueOf(name);
    if(auto current = m_trail->getCurrentAnimation())
        return current->getValueOf(name);
    return Entity::getValueOf(name);
}

void Ball::setValueOf(const std::string& name, const float value)
{
    if(m_trail == nullptr)
        return Entity::setValueOf(name, value);
    if(auto current = m_trail->getCurrentAnimation())
        return current->setValueOf(name, value);
    return Entity::setValueOf(name, value);
}

void Ball::draw(const DrawParameter& params)
{
    if(m_trail != nullptr)
        m_trail->draw(params);
    Entity::draw(params);
}

const Entity* Ball::getSpawnAnimationEntity() const
{
    return m_spawnAnimationEntity;
}

const Entity* Ball::getKillAnimationEntity() const
{
    return m_killAnimationEntity;
}

void Ball::blowUp()
{
    m_blownUp = true;
}

bool Ball::blownUp() const
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

bool Ball::getBallLost() const
{
    return m_lostBall;
}

void Ball::registerForCheckpointChanges(std::function<void()> handler)
{
    m_checkpointHandler = handler;
}

void Ball::setInvulnerable(const bool value)
{
    if(value)
        setValueOf("isInvulnerable", 1.0f);
    else
        setValueOf("isInvulnerable", 0.0f);
}

void Ball::bindTrail(std::unique_ptr<ParticleTrail> trail)
{
    m_trail = std::move(trail);
}

bool Ball::isSpeeding() const
{
    return m_trail != nullptr && getBody() != nullptr && getBody()->GetLinearVelocity().Length() >= m_trail->getSpeedMin();
}
