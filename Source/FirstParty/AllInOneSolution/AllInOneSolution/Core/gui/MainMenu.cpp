#include "MainMenu.hpp"

#include "../resources/ResourceManager.hpp"

MainMenu::MainMenu(const sf::Vector2f& position,
                   sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("MainMenu"), position, screen)
{
}
