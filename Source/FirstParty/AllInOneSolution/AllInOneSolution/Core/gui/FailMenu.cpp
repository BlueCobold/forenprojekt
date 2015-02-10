#include "FailMenu.hpp"

#include "../resources/ResourceManager.hpp"

FailMenu::FailMenu(sf::RenderWindow& screen,
                   ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("failMenu"), screen)
{
}
