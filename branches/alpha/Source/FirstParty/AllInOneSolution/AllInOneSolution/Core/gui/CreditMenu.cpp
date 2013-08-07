#include "CreditMenu.hpp"

#include "../resources/ResourceManager.hpp"

CreditMenu::CreditMenu(const sf::Vector2f& position,
                       sf::RenderWindow& screen,
                       ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("CreditMenu"), position, screen)
{
}