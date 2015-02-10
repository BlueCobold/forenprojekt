#include "MainMenu.hpp"

#include "../resources/ResourceManager.hpp"

MainMenu::MainMenu(sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("MainMenu"), screen)
{
}
