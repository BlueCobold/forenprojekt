#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Vector2f& position,
                   sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("MainMenu"), position, screen)
{
}
