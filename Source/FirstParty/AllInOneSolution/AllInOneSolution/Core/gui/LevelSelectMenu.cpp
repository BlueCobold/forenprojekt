#include "LevelSelectMenu.hpp"

#include "../resources/ResourceManager.hpp"

LevelSelectMenu::LevelSelectMenu(const sf::Vector2f& position,
                                 sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("LevelSelectMenu"), position, screen)
{
}
