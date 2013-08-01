#include "FailMenu.hpp"

#include "../resources/ResourceManager.hpp"

FailMenu::FailMenu(const sf::Vector2f& position,
                   sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("failMenu"), position, screen)
{
}
