#include "Cursor.hpp"
#include "../Input.hpp"

const std::string Cursor::TEXTURE_NAME = "GuiElements";
const int Cursor::SPRITE_POSITION_X = 910;
const int Cursor::SPRITE_POSITION_Y = 311;
const int Cursor::SPRITE_WIDTH = 38;
const int Cursor::SPRITE_HEIGHT = 48;

Cursor::Cursor(ResourceManager& resourceManager, const sf::RenderWindow& screen)
    : m_screen(screen)
{
    m_sprite.setTexture(*resourceManager.getTexture(TEXTURE_NAME));
    m_sprite.setTextureRect(sf::IntRect(SPRITE_POSITION_X, SPRITE_POSITION_Y, SPRITE_WIDTH, SPRITE_HEIGHT));
}

void Cursor::update()
{
    sf::Vector2i pos = sf::Mouse::getPosition(m_screen) - sf::Vector2i(6, 5);
    m_sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

void Cursor::draw(const DrawParameter& params)
{
    if(!utility::Mouse.isCursorVisible())
        return;
    params.getTarget().draw(m_sprite);
}