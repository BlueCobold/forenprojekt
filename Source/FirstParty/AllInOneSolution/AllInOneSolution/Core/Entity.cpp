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
    if(m_animation!=nullptr)
    {
        m_animation->update();
        m_sprite.setTextureRect(m_animation->getTextureRect());
    }

    m_sprite.setPosition(utility::toPixel(m_body->GetPosition().x), utility::toPixel(m_body->GetPosition().y));
    if(m_autoRotate)
        m_sprite.setRotation(utility::toDegree<float32, float>(m_body->GetAngle()));
}

void Entity::bindAnimation(std::unique_ptr<Animation> animation, bool autoRotate)
{
    m_autoRotate = autoRotate;
    m_animation = std::move(animation);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}
