#include "MainMenu.hpp"

#include "../resources/ResourceManager.hpp"
#include "Button.hpp"
#include "SubWindow.hpp"

MainMenu::MainMenu(sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("MainMenu"), screen)
{ }
