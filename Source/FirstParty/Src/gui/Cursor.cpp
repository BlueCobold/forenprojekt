#include "Cursor.hpp"
#include "../Input.hpp"

#include "../rendering/DrawParameter.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/SpriteSheet.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

const std::string SHEET_NAME = "gui_elements";
const std::string SPRITE_NAME = "Mouse";

Cursor::Cursor(ResourceManager& resourceManager, const sf::RenderWindow& screen) :
    m_screen(screen),
    m_sprite(sf::Sprite())
{
    auto sheet = resourceManager.getSpriteSheet(SHEET_NAME);
    auto iconSprite = sheet->get(SPRITE_NAME);
    m_sprite = Sprite(sf::Sprite(*resourceManager.getTexture(sheet->getTextureName())), iconSprite.blendMode);
    m_sprite.setTextureRect(sf::IntRect(iconSprite.x, iconSprite.y, iconSprite.width, iconSprite.height));
}

void Cursor::update()
{
#if defined(IOS) || defined(ANDROID)
    auto pos = utility::Mouse.getTouchPosition() - sf::Vector2i(6, 5);
#else
    auto pos = sf::Mouse::getPosition(m_screen) - sf::Vector2i(6, 5);
#endif

    m_sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

void Cursor::doDraw(const DrawParameter& params)
{
    if(!utility::Mouse.isCursorVisible())
        return;

    m_sprite.draw(params);
}
