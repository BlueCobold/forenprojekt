#include "LevelSelectMenu.hpp"

LevelSelectMenu::LevelSelectMenu(const sf::Vector2f& position,
                                 sf::RenderWindow& screen,
                                 ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("LevelSelectMenu"), position, screen)
{
}
