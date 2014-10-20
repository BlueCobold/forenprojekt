#include "Cursor.hpp"
#include "../Input.hpp"

const std::string SHEET_NAME = "gui_elements";
const std::string SPRITE_NAME = "Mouse";

Cursor::Cursor(ResourceManager& resourceManager, const sf::RenderWindow& screen)
    : m_screen(screen)
{
    auto sheet = resourceManager.getSpriteSheet(SHEET_NAME);
    auto iconSprite = sheet->get(SPRITE_NAME);
    m_sprite.setTexture(*resourceManager.getTexture(sheet->getTextureName()));
    m_sprite.setTextureRect(sf::IntRect(iconSprite.x, iconSprite.y, iconSprite.width, iconSprite.height));
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