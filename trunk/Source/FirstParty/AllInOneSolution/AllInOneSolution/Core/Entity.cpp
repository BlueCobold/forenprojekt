#include "Entity.hpp"
#include "Utility.hpp" // toDegree, toPixel

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
        getAnimation()->setPosition(utility::toPixel(m_body->GetPosition().x), utility::toPixel(m_body->GetPosition().y));
        getAnimation()->setRotation(static_cast<float32>(m_body->GetAngle()));
    }
}
