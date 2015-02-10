#include "LevelSelectMenu.hpp"
#include "Button.hpp"
#include "../resources/ResourceManager.hpp"

LevelSelectMenu::LevelSelectMenu(sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("LevelSelectMenu"), screen)
{
}

void LevelSelectMenu::setLevelPreview(const sf::Texture& texture, const sf::IntRect& textureRect)
{
    MenuSprite& sprite = Menu::getSprite(SPRITE_LEVELINFO);
    sprite.setTextureRect(textureRect);
    sprite.setTexture(texture);
}

void LevelSelectMenu::hideLeftButton(const bool value)
{
    Menu::getButton(BUTTON_LEFT).setVisible(!value);
}

void LevelSelectMenu::hideRightButton(const bool value)
{
    Menu::getButton(BUTTON_RIGHT).setVisible(!value);
}