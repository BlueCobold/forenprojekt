#include "PauseMenu.hpp"

#include "../resources/ResourceManager.hpp"

PauseMenu::PauseMenu(const sf::Vector2f& position,
                     sf::RenderWindow& screen,
                     ResourceManager& resourceManager) :
    CaptionMenu(*resourceManager.getMenuTemplate("pauseMenu"), position, screen)
{
}
