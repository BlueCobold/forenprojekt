#include "Entity.hpp"
//#include "Utility.hpp" // toDegree

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity()
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