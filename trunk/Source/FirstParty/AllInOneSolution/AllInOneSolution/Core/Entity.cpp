#include "Entity.hpp"
#include "Utility.hpp"

#include <exception>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Box2D/Dynamics/b2World.h>

Entity::Entity(Type type) : m_killed(false), m_type(type), m_updatingAni(nullptr)
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
        for(auto animation = getAnimations().begin(); animation != getAnimations().end(); animation++)
        {
            m_updatingAni = (*animation).get();
            (*animation)->setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
            (*animation)->setRotation(m_body->GetAngle());
            (*animation)->update();
        }
        m_updatingAni = nullptr;

        m_lastTime = value;
    }
}

float Entity::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't evaluate a variable at this time.");
    return m_updatingAni->getValueOf(name);
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

bool Entity::isCollideWithBall()
{
    return m_collideWithBall;
}