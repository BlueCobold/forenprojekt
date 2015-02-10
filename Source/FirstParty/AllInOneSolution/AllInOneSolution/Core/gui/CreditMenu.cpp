#include "CreditMenu.hpp"

#include "../resources/ResourceManager.hpp"

CreditMenu::CreditMenu(sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CreditMenu"), screen)
{
}