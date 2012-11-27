#include "Entity.hpp"

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
    if(getAnimation() != nullptr)
    {
        getAnimation()->update();
        getAnimation()->setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
        getAnimation()->setRotation(m_body->GetAngle());
    }
}
