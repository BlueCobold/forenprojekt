#include "Entity.hpp"
//#include "Utility.hpp" // toDegree

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity() :
    m_animated(false)
{
}

Entity::~Entity()
{
}

void Entity::update()
{
    //m_sprite.setPosition((m_body.GetPosition().x), (m_body.GetPosition().y));
    //m_sprite.setRotation(utility::toDegree<float32, float>(m_body.GetAngle()));
}

void Entity::update(const float value)
{
    if(m_animated)
    {
        m_animation.update(value);

        m_sprite.setTextureRect(m_animation.getTextureRect());
    }

    update();
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

void Entity::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}
void Entity::setRotation(float angle)
{
    m_sprite.setRotation(angle);
}

sf::Vector2<float> Entity::getPosition()
{
    return m_sprite.getPosition();
}

float Entity::getRotation()
{
    return m_sprite.getRotation();
}