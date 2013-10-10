#include "Cursor.hpp"
#include "../Input.hpp"

Cursor::Cursor(const sf::RenderWindow& screen)
    : m_screen(screen)
{
}

void Cursor::setResourceManager(ResourceManager& resourceManager)
{
    m_sprite.setTexture(*resourceManager.getTexture("cursor"));
}

void Cursor::update()
{
    sf::Vector2i pos = sf::Mouse::getPosition(m_screen);
    m_sprite.setPosition(pos.x, pos.y);
}

void Cursor::draw(const DrawParameter& params)
{
    if(!utility::Mouse.isCursorVisible())
        return;
    params.getTarget().draw(m_sprite);
}