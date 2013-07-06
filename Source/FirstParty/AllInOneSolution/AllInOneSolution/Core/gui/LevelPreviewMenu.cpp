#include "LevelPreviewMenu.hpp"

LevelPreviewMenu::LevelPreviewMenu(const sf::Vector2f& position,
                                   sf::RenderWindow& screen,
                                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("LevelPreviewMenu"), position, screen)
{
}

void LevelPreviewMenu::setLevelInfo(const std::string& levelName)
{
    Menu::getLabel(LABEL_LEVELNAME).setText(levelName);
}