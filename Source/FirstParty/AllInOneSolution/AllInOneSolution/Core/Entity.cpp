#include "Entity.hpp"

#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(Type type) : m_killed(false), m_type(type)
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