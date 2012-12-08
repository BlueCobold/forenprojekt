#include "Entity.hpp"

#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity() : m_killed(false)
{
}

Entity::~Entity()
{
}

void Entity::update(const float value)
{
    updateCurrentTime(value);
    updateKinematics(getPassedTime(), value - m_lastTime);
    if(getAnimation() != nullptr)
    {
        getAnimation()->setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
        getAnimation()->setRotation(m_body->GetAngle());
        getAnimation()->update();
    }

    m_lastTime = value;

}

void Entity::bindType(EntityType type)
{
    m_type = type;
}

void Entity::setName(std::string name)
{
    m_name = name;
}

EntityType& Entity::getType()
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

bool Entity::killed()
{
    return m_killed;
}