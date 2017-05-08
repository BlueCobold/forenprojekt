#include "FailMenu.hpp"

#include "../resources/ResourceManager.hpp"

FailMenu::FailMenu(sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("FailMenu"), screen)
{
}
