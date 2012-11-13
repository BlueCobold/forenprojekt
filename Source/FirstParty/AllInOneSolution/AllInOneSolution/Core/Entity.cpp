#include "Entity.hpp"
#include "Util.hpp"

void Entity::update()
{
    m_sprite.setPosition((m_body.GetPosition().x), (m_body.GetPosition().y));
    m_sprite.setRotation(Util::toDegree<float32, float>(m_body.GetAngle()));
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}
