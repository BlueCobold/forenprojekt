#include "CreditMenu.hpp"

#include "../resources/ResourceManager.hpp"

CreditMenu::CreditMenu(sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    Menu(*resourceManager.getMenuTemplate("CreditMenu"), screen)
{
}