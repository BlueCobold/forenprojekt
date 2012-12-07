#include "Entity.hpp"

#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity()
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
        getAnimation()->update();
        getAnimation()->setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
        getAnimation()->setRotation(m_body->GetAngle());
    }
    m_lastTime = value;
}
