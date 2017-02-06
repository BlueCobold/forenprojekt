#include "LevelSelectMenu.hpp"

#include "Button.hpp"
#include "MenuSprite.hpp"
#include "../resources/ResourceManager.hpp"

LevelSelectMenu::LevelSelectMenu(sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("LevelSelectMenu"), screen)
{
}

void LevelSelectMenu::setLevelPreview(const Sprite& sprite)
{
    MenuSprite& item = Menu::getSprite(SPRITE_LEVELINFO);
    item.setSprite(sprite);
}

void LevelSelectMenu::hideLeftButton(const bool value)
{
    Menu::getButton(BUTTON_LEFT).setVisible(!value);
}

void LevelSelectMenu::hideRightButton(const bool value)
{
    Menu::getButton(BUTTON_RIGHT).setVisible(!value);
}