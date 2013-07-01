#include "MenuSprite.hpp"

MenuSprite::MenuSprite() :
m_position(sf::Vector2f(0, 0)),
m_offset(sf::Vector2f(0, 0))
{
}
    
void MenuSprite::setOffset(sf::Vector2f offset)
{
    m_offset = offset;
    sf::Sprite::setPosition(m_offset + m_position);
}

sf::Vector2f MenuSprite::getOffset()
{
    return m_offset;
}

void MenuSprite::setPosition(sf::Vector2f position)
{
    m_position = position;
    sf::Sprite::setPosition(m_offset + m_position);
}

sf::Vector2f MenuSprite::getPosition()
{
    return m_position;
}

void MenuSprite::draw(const DrawParameter& params)
{
    params.getTarget().draw(*this);
}