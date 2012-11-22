#include "Entity.hpp"
#include "Utility.hpp" // toDegree, toPixel

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity() :
    m_animated(false)
{
}

Entity::~Entity()
{
}

void Entity::update(const float value)
{
    if(m_animated)
    {
        m_animation.update(value);

        m_sprite.setTextureRect(m_animation.getTextureRect());
    }

    m_sprite.setPosition(utility::toPixel(m_body->GetPosition().x), utility::toPixel(m_body->GetPosition().y));
    m_sprite.setRotation(utility::toDegree<float32, float>(m_body->GetAngle()));
}

void Entity::bindAnimation(const bool infinite, const float min, const float step,
    const unsigned int numFrames, const unsigned int frameWidth, const unsigned int frameHeight)
{
    m_animation = Animation(infinite, min, step, numFrames, frameWidth, frameHeight);
    m_animated = true;
}

Animation& Entity::getAnimation()
{
    return m_animation;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}
