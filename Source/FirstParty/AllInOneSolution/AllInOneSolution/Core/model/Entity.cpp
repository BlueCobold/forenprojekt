#include "Entity.hpp"
#include "../Utility.hpp"

#include <exception>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Box2D/Dynamics/b2World.h>

Entity::Entity(Type type) : m_killed(false), m_type(type), m_updatingAni(nullptr), m_collideWithBall(true)
{
}

Entity::~Entity()
{
}

void Entity::update(const float value)
{
    if(!m_killed)
    {
        updateCurrentTime(value);
        updateKinematics(getPassedTime(), value - m_lastTime);
        for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
        {
            m_updatingAni = (*animation).get();
            (*animation)->setPosition(utility::toPixel(m_body->GetPosition().x), utility::toPixel(m_body->GetPosition().y));
            (*animation)->setRotation(m_body->GetAngle());
            (*animation)->update();
        }
        m_updatingAni = nullptr;

        m_lastTime = value;
    }
}

float Entity::getValueOf(const std::string& name) const
{
    auto match = m_variables.find(name);
    if(match == end(m_variables))
    {
        if(m_updatingAni == nullptr)
            throw std::exception("Can't get a variable at this time.");
        return m_updatingAni->getValueOf(name);
    }
    return match->second;
}

void Entity::setValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        m_variables[name] = value;
    else
        m_updatingAni->setValueOf(name, value);
}

void Entity::setName(std::string name)
{
    m_name = name;
}

const Entity::Type& Entity::getType() const
{
    return m_type;
}

void Entity::restartAt(const float value)
{
    TimedObject::restartAt(value);
    m_lastTime = value;
}

void Entity::kill()
{    
    m_killed = true;
}

bool Entity::killed() const
{
    return m_killed;
}

void Entity::setCollideWithBall(bool value)
{
    m_collideWithBall = value;
}

bool Entity::doesCollideWithBall()
{
    return m_collideWithBall;
}

void Entity::bindCollisionHandler(std::unique_ptr<CollisionHandler> handler)
{
    m_collisionHandler.push_back(std::move(handler));
}

void Entity::bindCollisionFilter(std::unique_ptr<CollisionFilter> filter)
{
    m_collisionFilter = std::move(filter);
}

void Entity::onCollide(Entity* partner)
{
    for(auto it = begin(m_collisionHandler); it != end(m_collisionHandler); ++it)
        (*it)->OnCollision(this, partner);
}

bool Entity::shouldCollide(Entity* partner)
{
    if(m_collisionFilter == nullptr)
        return true;
    return m_collisionFilter->ShouldCollide(this, partner);
}