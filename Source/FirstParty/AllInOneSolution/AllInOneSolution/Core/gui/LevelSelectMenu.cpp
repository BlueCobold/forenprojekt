#include "LevelSelectMenu.hpp"
#include "Button.hpp"
#include "../resources/ResourceManager.hpp"

LevelSelectMenu::LevelSelectMenu(const sf::Vector2f& position,
                                 sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("LevelSelectMenu"), position, screen)
{
}
void LevelSelectMenu::setLevelTextureRect(const sf::IntRect& textureRect)
{
    Menu::getSprite(SPRITE_LEVELINFO).setTextureRect(textureRect);
}

void LevelSelectMenu::hideLeftButton(const bool value)
{
    Menu::getButton(BUTTON_LEFT).setVisible(!value);
}

void LevelSelectMenu::hideRightButton(const bool value)
{
    Menu::getButton(BUTTON_RIGHT).setVisible(!value);
}